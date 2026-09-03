#include "widget.h"
#include "ui_widget.h"
#include "gconfig.h"
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget),
      settings("Desktop.conf", QSettings::IniFormat),
      appDir(QCoreApplication::applicationDirPath()),  // 初始化应用程序目录
      appNameFont("Microsoft YaHei", appIconTextFontSize)
{
    ui->setupUi(this);
    qDebug() << "App directory:" << appDir;

    // 初始化缩放动画对象
    scaleAnimation = new QVariantAnimation(this);
    scaleAnimation->setDuration(animationDuration);
    scaleAnimation->setEasingCurve(QEasingCurve::OutBack);
    connect(scaleAnimation, &QVariantAnimation::valueChanged,
            this, &Widget::onScaleAnimationValueChanged);

    // 初始化子进程对象
    childProcess = new QProcess(this);
    connect(childProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Widget::onChildProcessFinished);
    connect(childProcess, &QProcess::errorOccurred,
            this, &Widget::onChildProcessError);

    // 预加载默认图标（内存优化：只加载一次）
    defaultAppIcon.load(":/images/app.png");
    if (!defaultAppIcon.isNull()) {
        defaultAppIcon = defaultAppIcon.scaled(appIconWidth, appIconHeight,
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    // 判断是否存在配置文件 ， 使用默认值，并创建配置文件
    // 如果存在就读取配置文件 ， 否则使用默认值并保存配置信息
    if (!settings.contains("Screen/Width")) {
        settings.setValue("Screen/Width", GCInstance.screenWidth);
    }
    if (!settings.contains("Screen/Height")) {
        settings.setValue("Screen/Height", GCInstance.screenHeight);
    }
    GCInstance.screenWidth = settings.value("Screen/Width").toInt();
    GCInstance.screenHeight = settings.value("Screen/Height").toInt();
    resize(GCInstance.screenWidth, GCInstance.screenHeight);

    // 加载应用程序的配置信息
    loadAppConfigList();
    
    // 预加载所有应用图标（内存优化：启动时一次性加载）
    preloadAppIcons();
    
    // 检测自启动应用并记录索引
    int autoStartID = settings.value("AutoStart/AutoStartID", 0).toInt();
    if (autoStartID != 0) {
        for (qint32 i = 0; i < appList.size(); i++) {
            if (appList[i].appID == autoStartID) {
                autoStartAppIndex = i;
                qDebug() << "Auto start app detected:" << appList[i].appName;
                break;
            }
        }
    }
    
    // 初始化 WiFi 状态检测定时器
    wifiCheckTimer = new QTimer(this);
    connect(wifiCheckTimer, &QTimer::timeout, this, &Widget::checkWifiStatus);
    wifiCheckTimer->start(10000);  // 每 10 秒检测一次
    
    // 立即检测一次 WiFi 状态
    checkWifiStatus();
}

Widget::~Widget()
{
    // scaleAnimation由Qt父子关系自动管理，无需手动删除
    delete ui;
}

// 检查应用索引是否有效
bool Widget::isValidAppIndex(qint32 index) const
{
    return index >= 0 && index < appList.size();
}

// 检查指定应用是否正在运行
bool Widget::isAppRunning(qint32 index) const
{
    if (!isValidAppIndex(index)) {
        return false;
    }
    return appList[index].isRunning;
}

// 检查是否有任何应用正在运行
bool Widget::hasAnyAppRunning() const
{
    // OTA 重启等待期间也视为"应用正在运行"，阻止触摸事件
    if (otaRestartPending) {
        return true;
    }
    return runningAppIndex >= 0 && childProcess && 
           childProcess->state() == QProcess::Running;
}

// 路径解析：相对路径转绝对路径（基于应用程序目录）
QString Widget::resolvePath(const QString &path) const
{
    if (path.isEmpty()) return QString();
    
    return QFileInfo(path).isAbsolute() 
           ? QDir::cleanPath(path)
           : QDir::cleanPath(QDir(appDir).absoluteFilePath(path));
}

// 加载应用程序的配置信息
void Widget::loadAppConfigList()
{
    appList.clear();  // 清空旧数据，支持重新加载
    
    for (int i = 1; i <= maxAppCount; i++)  // 添加上限保护，防止无限循环
    {
        QString groupName = QString("App%1").arg(i);
        settings.beginGroup(groupName);

        // 判断当前组是否存在ID键值
        if (!settings.contains("ID")) {
            settings.endGroup();
            break;
        }

        AppItem appItem;
        appItem.appID = settings.value("ID", 0).toInt();
        appItem.appName = settings.value("Name").toString();
        appItem.appIconPath = settings.value("Icon").toString();
        appItem.appExecPath = settings.value("Path").toString();

        // 验证必要字段
        if (appItem.appName.isEmpty()) {
            qWarning() << "App" << i << "has no name, skipping";
            settings.endGroup();
            continue;
        }

        qDebug() << "Loaded app:" << appItem.appID << appItem.appName;
        appList.append(appItem);

        settings.endGroup();
    }
    
    // 配置变化后需要重新计算矩形区域
    appRectsCalculated = false;
}

// 预加载应用程序图标（内存优化：启动时一次性加载到内存）
void Widget::preloadAppIcons()
{
    for (int i = 0; i < appList.size(); i++)
    {
        AppItem &item = appList[i];
        
        if (!item.appIconPath.isEmpty())
        {
            // 使用统一路径解析
            QString iconPath = resolvePath(item.appIconPath);
            
            if (QFileInfo::exists(iconPath))
            {
                item.cachedIcon.load(iconPath);
                if (!item.cachedIcon.isNull())
                {
                    item.cachedIcon = item.cachedIcon.scaled(appIconWidth, appIconHeight,
                                                            Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    item.iconLoaded = true;
                    qDebug() << "Loaded icon for" << item.appName << ":" << iconPath;
                }
                else
                {
                    qWarning() << "Failed to load icon:" << iconPath;
                }
            }
            else
            {
                qWarning() << "Icon not found:" << item.appIconPath << "-> resolved:" << iconPath;
            }
        }
        
        // 如果没有加载成功，使用默认图标
        if (!item.iconLoaded && !defaultAppIcon.isNull())
        {
            item.cachedIcon = defaultAppIcon;
            item.iconLoaded = true;
        }
    }
}

// 计算应用程序矩形区域（性能优化：只计算一次）
void Widget::calculateAppRects()
{
    if (appRectsCalculated) {
        return;  // 已经计算过，直接返回
    }
    
    appRectMap.clear();
    const int spacing = GCInstance.screenWidth / GCInstance.appIconCols - 15;

    for (qint32 i = 0; i < appList.size(); i++)
    {
        int row = i / GCInstance.appIconCols;
        int col = i % GCInstance.appIconCols;

        int currentAppX = spacing * col + appOffsetX;
        int currentAppY = spacing * row + appOffsetY;

        QRect appRect(currentAppX, currentAppY, appRectWidth, appRectHeight);
        appRectMap.insert(i, appRect);
    }
    
    appRectsCalculated = true;
}

// 检查是否有自启动应用
bool Widget::hasAutoStartApp() const
{
    return autoStartAppIndex >= 0 && isValidAppIndex(autoStartAppIndex);
}

// 直接启动自启动应用（不显示桌面）
void Widget::startWithAutoApp()
{
    if (!hasAutoStartApp()) {
        qWarning() << "No auto start app configured";
        showDesktop();
        return;
    }
    
    qDebug() << "Starting auto app directly:" << appList[autoStartAppIndex].appName;
    
    // 直接启动应用，不显示桌面
    if (!launchApp(autoStartAppIndex)) {
        qWarning() << "AutoStart failed for app:" << appList[autoStartAppIndex].appName;
        showDesktop();
    }
}

// 显示桌面
void Widget::showDesktop()
{
    desktopShown = true;
    // 恢复接收触摸/鼠标事件
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    show();
    raise();          // 确保窗口在最前面
    activateWindow(); // 激活窗口
    qDebug() << "Desktop shown";
}

// 自动运行应用程序（兼容原有逻辑，用于桌面已显示后的自启动）
void Widget::autoStart()
{
    if (!hasAutoStartApp()) {
        return;
    }
    
    if (!launchApp(autoStartAppIndex)) {
        qWarning() << "AutoStart failed for app:" << appList[autoStartAppIndex].appName;
    }
}

// 启动指定索引的应用程序（封装启动逻辑，便于错误处理）
bool Widget::launchApp(qint32 index)
{
    if (!isValidAppIndex(index)) {
        qWarning() << "Invalid app index:" << index;
        return false;
    }
    
    // 检查是否已有应用正在运行（不允许重复执行）
    if (hasAnyAppRunning()) {
        qWarning() << "Another app is already running, cannot launch:" << appList[index].appName;
        return false;
    }
    
    // 检查该应用是否已经在运行
    if (isAppRunning(index)) {
        qWarning() << "App is already running:" << appList[index].appName;
        return false;
    }
    
    AppItem &app = appList[index];
    
    if (app.appExecPath.isEmpty()) {
        qWarning() << "App exec path is empty:" << app.appName;
        return false;
    }
    
    // 使用统一路径解析
    QString execPath = resolvePath(app.appExecPath);
    
    // 检查可执行文件是否存在
    QFileInfo execFile(execPath);
    if (!execFile.exists()) {
        qWarning() << "Executable not found:" << app.appExecPath << "-> resolved:" << execPath;
        return false;
    }
    
    // 获取可执行文件所在目录作为工作目录
    QString workingDir = execFile.absolutePath();
    
    qDebug() << "Launching app:" << app.appName;
    qDebug() << "  Path:" << execPath;
    qDebug() << "  WorkDir:" << workingDir;
    
    // 设置工作目录
    childProcess->setWorkingDirectory(workingDir);
    
    // 确保子进程继承输入法相关环境变量
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("QT_IM_MODULE", "im");
    env.insert("QT_PLUGIN_PATH", "/usr/lib/qt/plugins");
    childProcess->setProcessEnvironment(env);
    
    qDebug() << "  QT_IM_MODULE:" << env.value("QT_IM_MODULE");
    qDebug() << "  QT_PLUGIN_PATH:" << env.value("QT_PLUGIN_PATH");
    
    // 转发子进程输出到控制台（用于调试）
    childProcess->setProcessChannelMode(QProcess::ForwardedChannels);
    
    // 检查 QProcess 状态，如果之前的进程还没完全清理，先等待
    if (childProcess->state() != QProcess::NotRunning) {
        qDebug() << "  Previous process still in state:" << childProcess->state();
        qDebug() << "  Waiting for process to finish...";
        childProcess->waitForFinished(1000);
        if (childProcess->state() != QProcess::NotRunning) {
            qWarning() << "  Process still running, killing...";
            childProcess->kill();
            childProcess->waitForFinished(1000);
        }
    }
    
    qDebug() << "  Starting process, current state:" << childProcess->state();
    childProcess->start(execPath);
    
    if (!childProcess->waitForStarted(3000)) {
        qWarning() << "Failed to start:" << app.appName;
        qWarning() << "  Error:" << childProcess->errorString();
        qWarning() << "  State:" << childProcess->state();
        return false;
    }
    
    qDebug() << "  Process started, new state:" << childProcess->state();
    
    // 标记应用正在运行
    app.isRunning = true;
    runningAppIndex = index;
    
    qDebug() << "App started successfully:" << app.appName << "PID:" << childProcess->processId();
    
    // 隐藏桌面，让子应用显示在最前面
    // 在 linuxfb 平台上，hide() 不会阻止触摸事件传递
    // 必须设置 WA_TransparentForMouseEvents 让桌面对触摸事件透明
    desktopShown = false;
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    hide();
    
    return true;
}

// 窗口大小变化事件（当窗口大小变化时重新缓存背景）
void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    
    // 窗口大小变化，需要重新缩放背景
    if (event->size() != lastWindowSize) {
        lastWindowSize = event->size();
        cachedBackground = QPixmap();  // 清空缓存，下次绘制时重新生成
    }
}

// 事件过滤：子程序运行期间阻止所有鼠标/触摸事件
bool Widget::event(QEvent *event)
{
    // 子程序运行期间，过滤掉所有鼠标和触摸相关事件
    if (hasAnyAppRunning()) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseMove:
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
        case QEvent::TouchCancel:
            // 直接丢弃这些事件，不传递给父类
            event->accept();
            return true;
        default:
            break;
        }
    }
    return QWidget::event(event);
}

// 绘制事件
void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    // 确保矩形区域已计算
    calculateAppRects();
    
    // 绘制背景图片
    drawBackground(painter);
    // 绘制apps的图标
    drawAppIcons(painter);
    // 绘制WiFi状态图标（右上角）
    drawWifiStatus(painter);
}

// 鼠标按压事件
void Widget::mousePressEvent(QMouseEvent *event)
{
    // 如果有应用正在运行，不响应鼠标事件
    if (hasAnyAppRunning()) {
        qDebug() << "Mouse event ignored: app is running";
        return;
    }
    
    // 确保矩形区域已计算
    calculateAppRects();
    
    // 检查是否点击了应用程序图标
    for (qint32 i = 0; i < appList.size(); i++)
    {
        if (appRectMap.contains(i) && appRectMap[i].contains(event->pos()))
        {
            qDebug() << "Clicked app:" << appList[i].appName;
            
            // 记录当前按下的应用程序索引
            pressedAppIndex = i;
            
            // 启动按下缩小动画（添加空指针检查）
            if (scaleAnimation) {
                scaleAnimation->stop();
                scaleAnimation->setStartValue(currentScale);
                scaleAnimation->setEndValue(pressedScale);
                scaleAnimation->setEasingCurve(QEasingCurve::OutQuad);
                scaleAnimation->start();
            }
            break;
        }
    }
}

// 鼠标释放事件
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    // 如果有应用正在运行，不响应鼠标事件
    if (hasAnyAppRunning()) {
        qDebug() << "Mouse event ignored: app is running";
        pressedAppIndex = -1;  // 重置按下状态
        return;
    }
    
    // 如果之前有按下的应用程序，启动恢复动画
    if (pressedAppIndex >= 0)
    {
        // 保存按下的索引，因为后面要重置
        qint32 releasedIndex = pressedAppIndex;
        
        // 启动释放恢复动画（添加空指针检查）
        if (scaleAnimation) {
            scaleAnimation->stop();
            scaleAnimation->setStartValue(currentScale);
            scaleAnimation->setEndValue(normalScale);
            scaleAnimation->setEasingCurve(QEasingCurve::OutBack);
            scaleAnimation->start();
        }

        // 检查是否在同一个图标上释放（有效点击）
        if (isValidAppIndex(releasedIndex) &&
            appRectMap.contains(releasedIndex) && 
            appRectMap[releasedIndex].contains(event->pos()))
        {
            qDebug() << "Released on app:" << appList[releasedIndex].appName;
            launchApp(releasedIndex);
        }

        // 重置按下的应用程序索引
        pressedAppIndex = -1;
    }
}

// 绘制背景图片（内存优化：缓存缩放后的背景）
void Widget::drawBackground(QPainter &painter)
{
    // 如果缓存的背景为空或尺寸不匹配，重新生成
    if (cachedBackground.isNull() || cachedBackground.size() != rect().size())
    {
        QPixmap originalBackground(":/images/back.png");
        if (!originalBackground.isNull()) {
            cachedBackground = originalBackground.scaled(rect().size(),
                                                         Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        } else {
            // 背景加载失败，使用纯色填充
            qWarning() << "Failed to load background image";
            painter.fillRect(rect(), Qt::darkGray);
            return;
        }
    }
    
    painter.drawPixmap(rect(), cachedBackground);
}

// 绘制apps的图标
void Widget::drawAppIcons(QPainter &painter)
{
    if (appList.isEmpty()) {
        return;  // 没有应用程序，直接返回
    }

    // 使用缓存的字体对象（性能优化）
    painter.setFont(appNameFont);
    painter.setPen(Qt::white);

    const int spacing = GCInstance.screenWidth / GCInstance.appIconCols - 15;

    for (qint32 i = 0; i < appList.size(); i++)
    {
        int row = i / GCInstance.appIconCols;
        int col = i % GCInstance.appIconCols;

        int currentAppX = spacing * col + appOffsetX;
        int currentAppY = spacing * row + appOffsetY;

        int offsetX = (appRectWidth - appIconWidth) / 2;

        // 创建图标和文本的矩形区域
        QRect iconRect(currentAppX + offsetX, currentAppY, appIconWidth, appIconHeight);
        QRect textRect(currentAppX + offsetX, iconRect.bottom(), appIconWidth, 30);

        // 绘制图标
        drawAppIcon(painter, i, iconRect);
        // 绘制文本
        painter.drawText(textRect, Qt::AlignCenter | Qt::AlignTop, appList[i].appName);
    }
}

// 绘制1个app的图标（内存优化：使用缓存的图标）
void Widget::drawAppIcon(QPainter &painter, qint32 i, const QRect &iconRect)
{
    // 边界检查（稳定性优化）
    if (!isValidAppIndex(i)) {
        return;
    }
    
    // 判断当前图标是否是被按下的图标
    bool isPressed = (i == pressedAppIndex);
    qreal scale = isPressed ? currentScale : normalScale;

    // 使用缓存的图标（内存优化：不再每次重新加载）
    const QPixmap &appIconPixmap = appList[i].iconLoaded ? appList[i].cachedIcon : defaultAppIcon;
    
    // 检查图标是否有效
    if (appIconPixmap.isNull()) {
        // 绘制占位符
        painter.fillRect(iconRect, Qt::gray);
        return;
    }

    // 如果是被按下的图标，应用缩放变换
    if (isPressed && qAbs(scale - normalScale) > 0.001)  // 使用浮点数比较（稳定性优化）
    {
        int scaledWidth = static_cast<int>(appIconWidth * scale);
        int scaledHeight = static_cast<int>(appIconHeight * scale);

        int offsetX = (appIconWidth - scaledWidth) / 2;
        int offsetY = (appIconHeight - scaledHeight) / 2;

        QRect scaledRect(iconRect.x() + offsetX, iconRect.y() + offsetY, 
                        scaledWidth, scaledHeight);

        painter.drawPixmap(scaledRect, appIconPixmap);
    }
    else
    {
        painter.drawPixmap(iconRect, appIconPixmap);
    }
}

// 缩放动画值变化槽函数
void Widget::onScaleAnimationValueChanged(const QVariant &value)
{
    currentScale = value.toReal();
    update();  // 触发重绘
}

// 子进程结束槽函数
void Widget::onChildProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // OTA 重启退出码
    const int OTA_RESTART_EXIT_CODE = 100;
    
    QString statusStr = (exitStatus == QProcess::NormalExit) ? "NormalExit" : "CrashExit";
    
    // 保存当前运行的应用索引（用于 OTA 重启）
    qint32 finishedAppIndex = runningAppIndex;
    
    if (isValidAppIndex(runningAppIndex)) {
        qDebug() << "App finished:" << appList[runningAppIndex].appName
                 << "ExitCode:" << exitCode
                 << "Status:" << statusStr;
        
        // 重置应用运行状态
        appList[runningAppIndex].isRunning = false;
    } else {
        qDebug() << "Child process finished, ExitCode:" << exitCode << "Status:" << statusStr;
    }
    
    // 检查是否为 OTA 重启请求（退出码 100）
    if (exitCode == OTA_RESTART_EXIT_CODE && exitStatus == QProcess::NormalExit) {
        qDebug() << "OTA restart requested, restarting app...";
        qDebug() << "  finishedAppIndex:" << finishedAppIndex;
        
        // 重置运行状态（重要：必须在这里重置，否则 launchApp 中的检查会失败）
        runningAppIndex = -1;
        
        // 设置 OTA 重启等待标志，阻止触摸事件响应
        otaRestartPending = true;
        
        // 延迟 500ms 后重启，确保资源释放
        QTimer::singleShot(500, this, [this, finishedAppIndex]() {
            qDebug() << "OTA: Timer fired, attempting to restart app";
            qDebug() << "  finishedAppIndex:" << finishedAppIndex
                     << "isValid:" << isValidAppIndex(finishedAppIndex);
            
            otaRestartPending = false;  // 清除等待标志
            
            if (isValidAppIndex(finishedAppIndex)) {
                qDebug() << "OTA: Calling launchApp for" << appList[finishedAppIndex].appName;
                bool success = launchApp(finishedAppIndex);
                qDebug() << "OTA: launchApp returned:" << success;
                
                if (!success) {
                    // 启动失败，恢复桌面
                    qWarning() << "OTA: Failed to restart app, showing desktop";
                    showDesktop();
                    update();
                }
            } else {
                // 索引无效，恢复桌面
                qWarning() << "OTA: Invalid app index, showing desktop";
                showDesktop();
                update();
            }
        });
        return;  // 不显示桌面，等待重启
    }
    
    // 重置运行状态（非 OTA 情况）
    runningAppIndex = -1;
    
    // 显示桌面（子应用退出后）
    showDesktop();
    update();
    
    qDebug() << "Desktop restored, ready for new app launch";
}

// 子进程错误槽函数
void Widget::onChildProcessError(QProcess::ProcessError error)
{
    QString errorStr;
    switch (error) {
        case QProcess::FailedToStart:
            errorStr = "FailedToStart";
            break;
        case QProcess::Crashed:
            errorStr = "Crashed";
            break;
        case QProcess::Timedout:
            errorStr = "Timedout";
            break;
        case QProcess::WriteError:
            errorStr = "WriteError";
            break;
        case QProcess::ReadError:
            errorStr = "ReadError";
            break;
        default:
            errorStr = "UnknownError";
            break;
    }
    
    if (isValidAppIndex(runningAppIndex)) {
        qWarning() << "App error:" << appList[runningAppIndex].appName << "Error:" << errorStr;
        appList[runningAppIndex].isRunning = false;
    } else {
        qWarning() << "Child process error:" << errorStr;
    }
    
    // 所有错误情况都需要重置状态并恢复桌面显示
    // 注意：Crashed 错误会同时触发 errorOccurred 和 finished 信号
    // FailedToStart 只触发 errorOccurred，不触发 finished
    if (error == QProcess::FailedToStart) {
        runningAppIndex = -1;
        showDesktop();
        update();
    }
    // 其他错误（如 Crashed）会在 onChildProcessFinished 中处理
}

// 检查 WiFi 连接状态
void Widget::checkWifiStatus()
{
    QProcess process;
    process.start("iwgetid", QStringList() << "-r");
    process.waitForFinished(3000);
    
    QString result = QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
    bool newStatus = !result.isEmpty();
    
    // 状态变化时才刷新界面
    if (newStatus != wifiConnected) {
        wifiConnected = newStatus;
        qDebug() << "WiFi status changed:" << (wifiConnected ? "Connected" : "Disconnected");
        update();  // 触发重绘
    }
}

// 绘制 WiFi 状态图标（右上角）
void Widget::drawWifiStatus(QPainter &painter)
{
    // 计算图标位置（右上角）
    int x = width() - wifiIconSize - wifiIconMargin;
    int y = wifiIconMargin;
    
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    if (wifiConnected) {
        // WiFi 已连接 - 绘制绿色 WiFi 图标
        painter.setPen(QPen(QColor(76, 175, 80), 3));  // 绿色
        
        // 绘制 WiFi 信号弧线（从外到内）
        int centerX = x + wifiIconSize / 2;
        int centerY = y + wifiIconSize;
        
        // 外层弧线
        painter.drawArc(centerX - 14, centerY - 28, 28, 28, 45 * 16, 90 * 16);
        // 中层弧线
        painter.drawArc(centerX - 10, centerY - 20, 20, 20, 45 * 16, 90 * 16);
        // 内层弧线
        painter.drawArc(centerX - 6, centerY - 12, 12, 12, 45 * 16, 90 * 16);
        // 中心点
        painter.setBrush(QColor(76, 175, 80));
        painter.drawEllipse(centerX - 3, centerY - 6, 6, 6);
    } else {
        // WiFi 未连接 - 绘制灰色 WiFi 图标带斜线
        painter.setPen(QPen(QColor(158, 158, 158), 3));  // 灰色
        
        // 绘制 WiFi 信号弧线
        int centerX = x + wifiIconSize / 2;
        int centerY = y + wifiIconSize;
        
        // 外层弧线
        painter.drawArc(centerX - 14, centerY - 28, 28, 28, 45 * 16, 90 * 16);
        // 中层弧线
        painter.drawArc(centerX - 10, centerY - 20, 20, 20, 45 * 16, 90 * 16);
        // 内层弧线
        painter.drawArc(centerX - 6, centerY - 12, 12, 12, 45 * 16, 90 * 16);
        // 中心点
        painter.setBrush(QColor(158, 158, 158));
        painter.drawEllipse(centerX - 3, centerY - 6, 6, 6);
        
        // 绘制红色斜线表示断开
        painter.setPen(QPen(QColor(244, 67, 54), 3));  // 红色
        painter.drawLine(x + 4, y + 4, x + wifiIconSize - 4, y + wifiIconSize - 4);
    }
    
    painter.restore();
}
