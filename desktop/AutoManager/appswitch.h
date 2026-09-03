/**
 * @file appswitch.h
 * @brief 应用程序开关控件头文件
 * 
 * 该文件定义了 appswitch 类，用于显示单个应用程序的开关控件。
 * 每个开关控件包含应用名称标签和一个可切换的按钮。
 * 
 * 优化特性：
 * - 使用静态图标缓存，所有实例共享同一图标资源
 * - 存储应用索引，避免在信号处理时遍历查找
 * - 提供静默设置状态的方法，避免触发不必要的信号
 */

#ifndef APPSWITCH_H
#define APPSWITCH_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>

// UI 命名空间前向声明
namespace Ui {
class appswitch;
}

/**
 * @class appswitch
 * @brief 应用程序开关控件类
 * 
 * 该类继承自 QWidget，用于显示一个应用程序的开关选项。
 * 包含一个显示应用名称的标签和一个可切换状态的按钮。
 * 当用户点击按钮时，会发出 switchStateChanged 信号。
 */
class appswitch : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为 nullptr
     */
    explicit appswitch(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     * 释放 UI 资源
     */
    ~appswitch();

    /**
     * @brief 设置应用程序信息
     * @param appName 应用程序名称，将显示在标签上
     * @param index 应用程序在列表中的索引，用于信号传递（默认为 -1）
     */
    void setAppInfo(const QString &appName, int index = -1);
    
    /**
     * @brief 获取开关按钮指针
     * @return 返回内部 QPushButton 指针，用于外部访问按钮状态
     */
    QPushButton* getPushButton() const;
    
    /**
     * @brief 获取应用程序索引
     * @return 返回当前控件对应的应用程序索引
     * @note 该方法用于快速定位，避免遍历查找（O(1) 复杂度）
     */
    int getAppIndex() const { return m_appIndex; }
    
    /**
     * @brief 静默设置开关状态
     * @param checked true 表示开启状态，false 表示关闭状态
     * @note 该方法仅更新 UI 状态，不会触发 switchStateChanged 信号
     *       用于程序内部状态同步，避免信号循环
     */
    void setCheckedState(bool checked);
    
    /**
     * @brief 设置共享图标（静态方法）
     * @param onIcon 开启状态的图标
     * @param offIcon 关闭状态的图标
     * @note 该方法允许外部预加载图标，所有实例将共享这些图标资源
     */
    static void setSharedIcons(const QIcon &onIcon, const QIcon &offIcon);

signals:
    /**
     * @brief 开关状态改变信号
     * @param index 发生状态改变的应用程序索引
     * @param checked 新的开关状态（true = 开启，false = 关闭）
     * @note 信号携带索引信息，接收方可直接使用索引定位，无需遍历查找
     */
    void switchStateChanged(int index, bool checked);

private slots:
    /**
     * @brief 按钮点击槽函数
     * @param checked 按钮的新状态
     * @note 由 Qt 自动连接机制调用（on_pushButton_clicked 命名约定）
     */
    void on_pushButton_clicked(bool checked);

private:
    Ui::appswitch *ui;          ///< UI 界面指针
    int m_appIndex = -1;        ///< 应用程序索引（-1 表示未设置）
    
    // ============ 静态图标缓存 ============
    // 优化说明：所有 appswitch 实例共享同一套图标资源
    // 避免每次创建实例或切换状态时重复加载图标
    static QIcon s_iconOn;              ///< 开启状态图标（静态共享）
    static QIcon s_iconOff;             ///< 关闭状态图标（静态共享）
    static bool s_iconsInitialized;     ///< 图标是否已初始化标志
};

#endif // APPSWITCH_H
