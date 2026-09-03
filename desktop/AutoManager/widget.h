/**
 * @file widget.h
 * @brief 主窗口头文件
 * 
 * 该文件定义了应用程序的主窗口类 Widget，用于管理和显示应用程序自启动设置。
 * 主窗口采用双列布局，左右各最多显示 5 个应用程序开关。
 * 
 * 功能特性：
 * - 从配置文件读取应用程序列表
 * - 显示应用程序开关控件
 * - 管理单选逻辑（同时只能有一个应用设为自启动）
 * - 保存用户的自启动选择到配置文件
 * 
 * 优化特性：
 * - 使用常量替代魔法数字，提高代码可维护性
 * - 使用前向声明减少头文件依赖
 * - 预分配容器内存，避免动态扩容
 * - 缓存当前自启动索引，避免重复遍历
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QVector>
#include <QPushButton>

// ============ 前向声明 ============
// 使用前向声明替代 #include "appswitch.h"
// 优点：减少头文件依赖，加快编译速度，降低重编译范围
// 注意：仅当头文件中只使用指针/引用时可用前向声明
class appswitch;

// Qt UI 命名空间
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

/**
 * @struct AppItem
 * @brief 应用程序配置信息结构体
 * 
 * 存储从配置文件读取的单个应用程序信息。
 * 使用 struct 而非 class，因为这是一个纯数据容器（POD-like）。
 */
struct AppItem
{
    qint32 appID = 0;         ///< 应用程序唯一标识符
    QString appName;          ///< 应用程序显示名称
    QString appIconPath;      ///< 应用程序图标文件路径
    QString appExecPath;      ///< 应用程序可执行文件路径
};

/**
 * @class Widget
 * @brief 应用程序管理器主窗口类
 * 
 * 该类是应用程序的主界面，负责：
 * 1. 初始化和显示 UI
 * 2. 从配置文件加载应用程序列表
 * 3. 创建和管理应用程序开关控件
 * 4. 处理用户交互（开关切换、关闭窗口）
 * 5. 保存用户设置到配置文件
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为 nullptr（顶级窗口）
     */
    explicit Widget(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     * 释放 UI 资源
     */
    ~Widget();

private slots:
    /**
     * @brief 关闭按钮点击槽函数
     * 当用户点击关闭按钮时关闭窗口
     */
    void closeButton_clicked_slot();
    
    /**
     * @brief 应用开关状态改变槽函数
     * @param index 发生状态改变的应用程序索引
     * @param checked 新的开关状态
     * 
     * 处理逻辑：
     * - 如果开启：关闭之前选中的开关，更新配置文件
     * - 如果关闭：清除自启动设置
     */
    void switchStateChanged_slot(int index, bool checked);

private:
    // ============ 初始化方法 ============
    // 将构造函数的逻辑拆分为多个独立方法
    // 优点：提高代码可读性，便于单独测试和维护
    
    /**
     * @brief 初始化用户界面
     * 设置窗口大小、样式、创建关闭按钮等
     */
    void initUI();
    
    /**
     * @brief 加载应用程序配置
     * 从配置文件读取所有应用程序信息
     */
    void loadAppConfigs();
    
    /**
     * @brief 创建应用程序开关控件
     * 根据加载的配置创建并布局所有开关控件
     */
    void createAppSwitches();
    
    /**
     * @brief 加载自启动状态
     * 读取配置文件中的自启动设置，并设置对应开关的状态
     */
    void loadAutoStartState();
    
    // ============ 辅助方法 ============
    
    /**
     * @brief 创建单个开关控件
     * @param parent 父控件指针
     * @param index 应用程序索引
     * @return 创建的 appswitch 控件指针
     */
    appswitch* createSwitchWidget(QWidget *parent, int index);

private:
    Ui::Widget *ui;         ///< UI 界面指针
    QSettings settings;     ///< 配置文件读写对象
    
    // ============ 配置常量 ============
    // 使用 constexpr 定义编译期常量，替代代码中的"魔法数字"
    // 优点：便于修改、提高可读性、避免错误
    
    static constexpr int MAX_APP_COUNT = 10;        ///< 最大应用程序数量
    static constexpr int ITEMS_PER_COLUMN = 5;      ///< 每列显示的项目数
    static constexpr int ITEM_HEIGHT = 80;          ///< 列表项高度（像素）
    static constexpr int TOP_MARGIN = 76;           ///< 顶部边距（像素）
    static constexpr int CLOSE_BUTTON_SIZE = 76;    ///< 关闭按钮尺寸（像素）
    static constexpr int CLOSE_ICON_SIZE = 50;      ///< 关闭按钮图标尺寸（像素）
    
    // ============ 数据容器 ============
    // 使用 QVector 存储动态数据，构造时使用 reserve() 预分配内存
    
    QVector<AppItem> appList;           ///< 应用程序配置列表
    QVector<appswitch*> switchWidgets;  ///< 开关控件指针列表
    
    // ============ 状态缓存 ============
    // 缓存当前自启动应用的索引，避免每次操作都遍历查找
    
    int m_currentAutoStartIndex = -1;   ///< 当前自启动应用索引（-1 表示无）
};

#endif // WIDGET_H
