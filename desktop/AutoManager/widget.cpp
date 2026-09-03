/**
 * @file widget.cpp
 * @brief 主窗口实现文件
 * 
 * 该文件实现了 Widget 类的所有成员函数，包括：
 * - 窗口初始化
 * - 配置文件读取
 * - 开关控件创建
 * - 用户交互处理
 */

#include "widget.h"
#include "ui_widget.h"
#include "appswitch.h"
#include "closebutton.h"
#include <QAbstractItemView>
#include <QFrame>
#include <QMessageBox>
#include <QProcess>

// ============ 匿名命名空间 ============
// 用于定义文件作用域的常量，避免全局命名污染
namespace {
    // 主窗口样式 - 白色背景
    const QString WINDOW_STYLE = QStringLiteral(
        "QWidget { background-color: #FFFFFF; }"
    );
    
    // 列表控件样式表 - 现代简洁风格
    const QString LIST_STYLE = QStringLiteral(
        "QListWidget {"
        "    background-color: #FFFFFF;"
        "    border: none;"
        "    border-left: none;"
        "    border-right: none;"
        "    margin-top: 76px;"
        "    outline: none;"
        "}"
        "QListWidget::item {"
        "    background: transparent;"
        "    border: none;"
        "    border-bottom: 1px solid #EEEEEE;"
        "    padding: 8px 0px;"
        "}"
        "QListWidget::item:last {"
        "    border-bottom: none;"
        "}"
        "QListWidget::item:selected { background: transparent; }"
        "QListWidget::item:hover { background: #FAFAFA; }"
        "QListWidget::item:focus { background: transparent; }"
    );
    
}

/**
 * @brief Widget 构造函数
 * 
 * 初始化流程：
 * 1. 调用父类构造函数
 * 2. 创建 UI
 * 3. 初始化配置文件读取器
 * 4. 预分配容器内存
 * 5. 依次调用初始化方法
 */
Widget::Widget(QWidget *parent)
    : QWidget(parent)                                              // 调用父类构造函数
    , ui(new Ui::Widget)                                           // 创建 UI 对象
    , settings("../MicroDesktop/Desktop.conf", QSettings::IniFormat)  // 初始化配置文件
{
    // 加载 UI 设计文件
    ui->setupUi(this);
    
    // ============ 预分配容器内存 ============
    // 使用 reserve() 预先分配足够的内存空间
    // 优点：避免 QVector 在 append() 时触发动态扩容和内存拷贝
    // 这对于已知最大元素数量的场景是重要的性能优化
    appList.reserve(MAX_APP_COUNT);
    switchWidgets.reserve(MAX_APP_COUNT);
    
    // 按顺序执行初始化步骤
    // 注意：这些方法有依赖关系，必须按此顺序调用
    initUI();              // 1. 初始化界面
    loadAppConfigs();      // 2. 加载应用配置
    createAppSwitches();   // 3. 创建开关控件（依赖步骤2）
    loadAutoStartState();  // 4. 恢复自启动状态（依赖步骤3）
}

/**
 * @brief Widget 析构函数
 * 释放 UI 资源，子控件由 Qt 父子对象机制自动释放
 */
Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 初始化用户界面
 * 
 * 负责：
 * 1. 配置列表控件（隐藏滚动条、设置样式）
 * 2. 设置窗口大小
 * 3. 创建关闭按钮
 */
void Widget::initUI()
{
    // ============ 设置窗口背景为白色 ============
    setStyleSheet(WINDOW_STYLE);
    
    // ============ 设置窗口尺寸 ============
    const int width = settings.value("Screen/Width").toInt();
    const int height = settings.value("Screen/Height").toInt();
    resize(width, height);
    
    // ============ 配置列表控件 ============
    // 隐藏滚动条
    ui->listWidget_left->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget_right->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget_left->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget_right->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // 禁用选择和焦点
    ui->listWidget_left->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listWidget_right->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listWidget_left->setFocusPolicy(Qt::NoFocus);
    ui->listWidget_right->setFocusPolicy(Qt::NoFocus);
    
    // 设置列表控件框架为无边框
    ui->listWidget_left->setFrameShape(QFrame::NoFrame);
    ui->listWidget_right->setFrameShape(QFrame::NoFrame);
    
    // ============ 设置列表样式 ============
    // 添加顶部边距，为关闭按钮留出空间
    // 使用预定义的常量字符串，避免重复创建
    ui->listWidget_left->setStyleSheet(LIST_STYLE);
    ui->listWidget_right->setStyleSheet(LIST_STYLE);
    
    // ============ 创建关闭按钮 ============
    // 使用自定义的现代风格关闭按钮
    auto *closeButton = new CloseButton(this);
    closeButton->setButtonSize(CLOSE_ICON_SIZE);
    closeButton->move(width - CLOSE_ICON_SIZE - 15, 15);
    connect(closeButton, &CloseButton::clicked, this, &Widget::closeButton_clicked_slot);
}

/**
 * @brief 加载应用程序配置
 * 
 * 从配置文件读取所有应用程序信息，存入 appList 容器。
 * 
 * 配置文件格式（INI）：
 * [App1]
 * ID=1
 * Name=应用名称
 * Icon=图标路径
 * Path=执行路径
 * 
 * [App2]
 * ...
 */
void Widget::loadAppConfigs()
{
    // 遍历可能存在的应用程序配置节
    // 从 App1 开始，直到 App{MAX_APP_COUNT} 或遇到不存在的节
    for (int i = 1; i <= MAX_APP_COUNT; ++i)
    {
        // 构建配置节名称，如 "App1", "App2", ...
        // 使用 QStringLiteral 优化字符串创建
        const QString groupName = QStringLiteral("App%1").arg(i);
        settings.beginGroup(groupName);  // 进入配置节
        
        // 检查该配置节是否存在（通过检查 ID 键）
        if (!settings.contains("ID"))
        {
            settings.endGroup();  // 必须调用 endGroup() 与 beginGroup() 配对
            break;  // 没有更多应用程序，退出循环
        }
        
        // 读取应用程序信息
        AppItem item;
        item.appID = settings.value("ID", 0).toInt();        // ID，默认值 0
        item.appName = settings.value("Name").toString();    // 名称
        item.appIconPath = settings.value("Icon").toString();// 图标路径
        item.appExecPath = settings.value("Path").toString();// 执行路径
        
        // 使用 std::move 将临时对象移动到容器中
        // 优化：避免不必要的拷贝，特别是 QString 成员
        appList.append(std::move(item));
        settings.endGroup();  // 退出配置节
    }
    
    // ============ 调试输出 ============
    // 仅在调试版本中输出加载的应用信息
    // 使用条件编译，Release 版本不包含这些代码
#ifdef QT_DEBUG
    for (const auto &app : appList)
    {
        qDebug() << "Loaded app:" << app.appID << app.appName;
    }
#endif
}

/**
 * @brief 创建单个开关控件
 * 
 * 工厂方法，封装开关控件的创建和初始化逻辑。
 * 
 * @param parent 父控件（决定控件放在哪个列表中）
 * @param index 应用程序在 appList 中的索引
 * @return 创建好的 appswitch 控件指针
 */
appswitch* Widget::createSwitchWidget(QWidget *parent, int index)
{
    // 创建开关控件
    auto *switchWidget = new appswitch(parent);
    
    // 设置应用信息和索引
    // 索引将用于信号传递，实现 O(1) 复杂度的查找
    switchWidget->setAppInfo(appList[index].appName, index);
    
    // 连接信号槽
    // 使用新式语法：&ClassName::signalName
    // 优点：编译期检查，类型安全
    connect(switchWidget, &appswitch::switchStateChanged, 
            this, &Widget::switchStateChanged_slot);
    
    return switchWidget;
}

/**
 * @brief 创建所有应用程序开关控件
 * 
 * 遍历 appList，为每个应用创建开关控件并添加到对应的列表中。
 * 前 5 个应用放在左侧列表，后 5 个放在右侧列表。
 */
void Widget::createAppSwitches()
{
    const int appCount = appList.size();
    
    for (int i = 0; i < appCount; ++i)
    {
        // ============ 选择目标列表 ============
        // 使用三元运算符简化分支逻辑
        // 索引 0-4 放左侧，5-9 放右侧
        QListWidget *targetList = (i < ITEMS_PER_COLUMN) 
            ? ui->listWidget_left 
            : ui->listWidget_right;
        
        // ============ 创建列表项 ============
        auto *item = new QListWidgetItem(targetList);
        // 设置项目尺寸（宽度留 2px 边距）
        item->setSizeHint(QSize(400 - 2, ITEM_HEIGHT));
        targetList->addItem(item);
        
        // ============ 创建并关联开关控件 ============
        appswitch *switchWidget = createSwitchWidget(targetList, i);
        // 将自定义控件设置为列表项的显示内容
        targetList->setItemWidget(item, switchWidget);
        
        // 保存控件指针，用于后续状态管理
        switchWidgets.append(switchWidget);
    }
}

/**
 * @brief 加载自启动状态
 * 
 * 从配置文件读取 AutoStartID，找到对应的开关并设置为选中状态。
 * 
 * 配置文件格式：
 * [AutoStart]
 * AutoStartID=1  // 对应 App1 的 ID
 */
void Widget::loadAutoStartState()
{
    // 读取自启动 ID
    settings.beginGroup("AutoStart");
    const qint32 autoStartID = settings.value("AutoStartID", 0).toInt();
    settings.endGroup();
    
    // ID <= 0 表示没有设置自启动
    if (autoStartID <= 0)
    {
        m_currentAutoStartIndex = -1;
        return;
    }
    
    // 查找匹配的应用程序
    // 注意：appID 和数组索引可能不一致，需要遍历查找
    for (int i = 0; i < appList.size(); ++i)
    {
        if (autoStartID == appList[i].appID)
        {
            // 找到匹配项，缓存索引并设置开关状态
            m_currentAutoStartIndex = i;
            // 使用 setCheckedState 静默设置，不触发信号
            switchWidgets[i]->setCheckedState(true);
            break;
        }
    }
}

/**
 * @brief 开关状态改变槽函数
 * 
 * 当用户点击任意开关时调用。实现单选逻辑：
 * - 同一时间只能有一个应用设为自启动
 * - 点击已选中的开关会取消自启动
 * 
 * @param index 被操作的开关索引（由信号传入）
 * @param checked 新的开关状态
 */
void Widget::switchStateChanged_slot(int index, bool checked)
{
    // ============ 参数校验 ============
    // 防止越界访问
    if (index < 0 || index >= switchWidgets.size())
        return;
    
    if (checked)
    {
        // ============ 开启开关 ============
        
        // 关闭之前选中的开关（实现单选逻辑）
        // 直接通过缓存的索引访问，O(1) 复杂度
        if (m_currentAutoStartIndex >= 0 && m_currentAutoStartIndex != index)
        {
            // 静默关闭，不触发信号，避免递归
            switchWidgets[m_currentAutoStartIndex]->setCheckedState(false);
        }
        
        // 更新状态缓存
        m_currentAutoStartIndex = index;
        
        // 保存到配置文件
        // 存储的是应用的 ID（不是索引），以适应配置变化
        settings.beginGroup("AutoStart");
        settings.setValue("AutoStartID", appList[index].appID);
        settings.endGroup();
    }
    else
    {
        // ============ 关闭开关 ============
        
        // 仅当关闭的是当前自启动项时才清除设置
        if (m_currentAutoStartIndex == index)
        {
            m_currentAutoStartIndex = -1;  // 清除缓存
            
            // 保存到配置文件（设为 0 表示无自启动）
            settings.beginGroup("AutoStart");
            settings.setValue("AutoStartID", 0);
            settings.endGroup();
        }
    }
}

/**
 * @brief 关闭按钮点击槽函数
 * 弹出对话框询问用户是立即重启系统还是稍后重启
 */
void Widget::closeButton_clicked_slot()
{
    // 创建消息对话框
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("系统重启");
    msgBox.setText("配置已保存，是否立即重启系统使设置生效？");
    msgBox.setIcon(QMessageBox::NoIcon);  // 不显示图标
    
    // 设置对话框大小和样式
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #FFFFFF;"
        "}"
        "QMessageBox QLabel {"
        "    font-size: 20px;"
        "    padding: 20px;"
        "    min-width: 400px;"
        "    min-height: 60px;"
        "}"
        "QMessageBox QPushButton {"
        "    font-size: 18px;"
        "    padding: 12px 40px;"
        "    min-width: 120px;"
        "    min-height: 40px;"
        "    border-radius: 8px;"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "}"
        "QMessageBox QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
    );
    
    // 设置固定大小
    msgBox.setFixedSize(600, 200);
    
    // 添加自定义按钮
    QPushButton *rebootBtn = msgBox.addButton("立即重启", QMessageBox::AcceptRole);
    QPushButton *laterBtn = msgBox.addButton("稍后重启", QMessageBox::RejectRole);
    
    // 设置按钮样式区分
    rebootBtn->setStyleSheet(
        "QPushButton {"
        "    font-size: 18px;"
        "    padding: 12px 40px;"
        "    min-width: 120px;"
        "    min-height: 40px;"
        "    border-radius: 8px;"
        "    background-color: #f44336;"
        "    color: white;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #da190b;"
        "}"
    );
    
    laterBtn->setStyleSheet(
        "QPushButton {"
        "    font-size: 18px;"
        "    padding: 12px 40px;"
        "    min-width: 120px;"
        "    min-height: 40px;"
        "    border-radius: 8px;"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
    );
    
    // 设置默认按钮
    msgBox.setDefaultButton(laterBtn);
    
    // 显示对话框并等待用户选择
    msgBox.exec();
    
    if (msgBox.clickedButton() == rebootBtn)
    {
        // 用户选择立即重启
        QProcess::startDetached("reboot");
    }
    else
    {
        // 用户选择稍后重启，退出程序
        close();
        qApp->exit();
    }
}
