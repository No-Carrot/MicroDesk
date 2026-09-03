#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QMap>
#include <QMouseEvent>
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QPixmapCache>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

// 存储单个应用程序的配置信息
class AppItem
{
public:
    qint32 appID = 0;         // 应用程序ID
    QString appName;          // 应用程序名称
    QString appIconPath;      // 应用程序图标路径
    QString appExecPath;      // 应用程序执行路径
    QPixmap cachedIcon;       // 缓存的图标（优化：避免重复加载）
    bool iconLoaded = false;  // 图标是否已加载
    bool isRunning = false;   // 应用程序是否正在运行
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    
    bool hasAutoStartApp() const;  // 检查是否有自启动应用
    void startWithAutoApp();       // 直接启动自启动应用（不显示桌面）
    void showDesktop();            // 显示桌面

protected:
    bool event(QEvent *event) override;                 // 事件过滤（阻止子程序运行时的触摸事件）
    void paintEvent(QPaintEvent *event) override;       // 绘制事件
    void mousePressEvent(QMouseEvent *event) override;  // 鼠标按压事件
    void mouseReleaseEvent(QMouseEvent *event) override;// 鼠标释放事件
    void resizeEvent(QResizeEvent *event) override;     // 窗口大小变化事件

private:
    // ========== 绘制相关方法 ======================
    void drawBackground(QPainter &painter);                       // 绘制背景图片
    void drawAppIcon(QPainter &painter, qint32 i, const QRect &iconRect); // 绘制1个app的图标
    void drawAppIcons(QPainter &painter);                         // 绘制apps的图标
    void drawWifiStatus(QPainter &painter);                       // 绘制WiFi状态图标

    // ========== 初始化和配置方法 ======================
    void loadAppConfigList();    // 加载应用程序配置列表
    void preloadAppIcons();      // 预加载应用程序图标（优化：启动时一次性加载）
    void calculateAppRects();    // 计算应用程序矩形区域（优化：只在需要时计算一次）
    void autoStart();            // 自动运行应用程序
    bool launchApp(qint32 index);// 启动指定索引的应用程序

    // ========== 辅助方法 ======================
    bool isValidAppIndex(qint32 index) const;  // 检查应用索引是否有效
    QString resolvePath(const QString &path) const;  // 路径解析（相对路径转绝对路径）
    bool isAppRunning(qint32 index) const;     // 检查指定应用是否正在运行
    bool hasAnyAppRunning() const;             // 检查是否有任何应用正在运行

private:
    Ui::Widget *ui;

    // ========== UI 布局常量定义 ======================
    static constexpr int appIconWidth = 100;       // 应用程序图标宽度
    static constexpr int appIconHeight = 100;      // 应用程序图标高度
    static constexpr int appRectWidth = 130;       // 应用程序矩形的宽度
    static constexpr int appRectHeight = 130;      // 应用程序矩形的高度
    static constexpr int appOffsetX = 55;          // 应用程序图标水平偏移量
    static constexpr int appOffsetY = 80;          // 应用程序图标垂直偏移量
    static constexpr int appIconTextFontSize = 16; // 应用程序图标文本字体大小
    static constexpr int maxAppCount = 100;        // 最大应用程序数量限制（防止配置错误导致死循环）

    // ========== 成员变量 ======================
    QVector<AppItem> appList;          // 应用程序列表
    QMap<int, QRect> appRectMap;       // 应用程序矩形框映射表
    bool appRectsCalculated = false;   // 矩形区域是否已计算（优化：避免重复计算）
    
    QSettings settings;                // qsettings 配置文件
    QString appDir;                    // 应用程序目录（路径解析基准）

    // ========== 缓存的资源（内存优化）======================
    QPixmap cachedBackground;          // 缓存的背景图片
    QPixmap defaultAppIcon;            // 缓存的默认应用图标
    QSize lastWindowSize;              // 上次窗口大小（用于判断是否需要重新缩放背景）
    QFont appNameFont;                 // 缓存的字体对象

    // ========== 动画相关成员变量 ======================
    qint32 pressedAppIndex = -1;       // 当前按下的应用程序索引，-1表示没有按下
    qreal currentScale = 1.0;          // 当前图标缩放比例
    QVariantAnimation *scaleAnimation = nullptr; // 缩放动画对象（初始化为nullptr）

    // 动画常量配置
    static constexpr qreal pressedScale = 0.85;   // 按下时的缩放比例
    static constexpr qreal normalScale = 1.0;     // 正常状态的缩放比例
    static constexpr int animationDuration = 100; // 动画持续时间(毫秒)

    // ========== 子进程管理 ======================
    QProcess *childProcess = nullptr;  // 子进程对象
    qint32 runningAppIndex = -1;       // 当前运行的应用程序索引，-1表示没有运行
    qint32 autoStartAppIndex = -1;     // 自启动应用的索引，-1表示没有自启动
    bool desktopShown = false;         // 桌面是否已显示（用于控制首次显示逻辑）
    bool otaRestartPending = false;    // OTA 重启等待中（阻止触摸事件响应）

    // ========== WiFi 状态检测 ======================
    QTimer *wifiCheckTimer = nullptr;  // WiFi 状态检测定时器
    bool wifiConnected = false;        // WiFi 是否已连接
    static constexpr int wifiIconSize = 32;      // WiFi 图标大小
    static constexpr int wifiIconMargin = 15;    // WiFi 图标边距

private slots:
    void onScaleAnimationValueChanged(const QVariant &value); // 缩放动画值变化槽函数
    void onChildProcessFinished(int exitCode, QProcess::ExitStatus exitStatus); // 子进程结束槽函数
    void onChildProcessError(QProcess::ProcessError error);   // 子进程错误槽函数
    void checkWifiStatus();  // 检查 WiFi 连接状态
};
#endif // WIDGET_H
