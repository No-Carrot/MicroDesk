/**
 * @file appswitch.cpp
 * @brief 应用程序开关控件实现文件
 * 
 * 该文件实现了 appswitch 类的所有成员函数。
 * 包括构造/析构函数、状态设置、信号发射等功能。
 */

#include "appswitch.h"
#include "ui_appswitch.h"

// ============ 静态成员变量初始化 ============
// 静态成员需要在类外部定义和初始化
QIcon appswitch::s_iconOn;              // 开启状态图标
QIcon appswitch::s_iconOff;             // 关闭状态图标
bool appswitch::s_iconsInitialized = false;  // 初始化标志，默认未初始化

/**
 * @brief 构造函数实现
 * 
 * 初始化流程：
 * 1. 调用父类构造函数
 * 2. 创建并设置 UI
 * 3. 懒加载共享图标（仅首次创建时）
 * 4. 设置按钮初始状态和样式
 */
appswitch::appswitch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::appswitch)
{
    // 加载 UI 设计文件
    ui->setupUi(this);
    
    // ============ 设置控件背景透明 ============
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background: transparent;");
    
    // ============ 设置标签样式 ============
    // 现代化字体样式：深灰色文字，适中字号
    ui->label->setStyleSheet(QStringLiteral(
        "QLabel {"
        "    color: #333333;"
        "    font-size: 18px;"
        "    font-weight: 500;"
        "    background: transparent;"
        "    padding-left: 15px;"
        "}"
    ));
    
    // ============ 设置按钮样式 ============
    ui->pushButton->setCheckable(true);
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    
    // 懒加载图标
    if (!s_iconsInitialized) {
        s_iconOn = QIcon(":/images/switchon.png");
        s_iconOff = QIcon(":/images/switchoff.png");
        s_iconsInitialized = true;
    }
    
    ui->pushButton->setIcon(s_iconOff);
    ui->pushButton->setIconSize(QSize(80, 40));  // 稍微缩小图标尺寸
    ui->pushButton->setStyleSheet(QStringLiteral(
        "QPushButton { border: none; background: transparent; }"
    ));
}

/**
 * @brief 析构函数实现
 * 释放 UI 资源，由 Qt 父子对象机制自动管理子控件
 */
appswitch::~appswitch()
{
    delete ui;
}

/**
 * @brief 设置共享图标
 * 
 * 允许外部代码预加载图标资源，用于自定义图标场景。
 * 调用此方法后，所有 appswitch 实例将使用新的图标。
 */
void appswitch::setSharedIcons(const QIcon &onIcon, const QIcon &offIcon)
{
    s_iconOn = onIcon;
    s_iconOff = offIcon;
    s_iconsInitialized = true;
}

/**
 * @brief 设置应用程序信息
 * 
 * 将应用名称显示在标签上，并记录索引用于后续信号传递。
 * 索引的使用避免了在信号处理时需要遍历查找发送者。
 */
void appswitch::setAppInfo(const QString &appName, int index)
{
    ui->label->setText(appName);  // 设置应用名称标签
    m_appIndex = index;           // 保存索引供信号使用
}

/**
 * @brief 静默设置开关状态
 * 
 * 该方法用于程序内部同步状态，不触发 switchStateChanged 信号。
 * 
 * 实现原理：
 * 1. 使用 blockSignals(true) 临时阻止信号发射
 * 2. 更新按钮的 checked 状态和图标
 * 3. 使用 blockSignals(false) 恢复信号发射
 * 
 * 使用场景：
 * - 初始化时恢复保存的状态
 * - 单选逻辑中关闭其他开关
 * - 避免信号循环导致的无限递归
 */
void appswitch::setCheckedState(bool checked)
{
    ui->pushButton->blockSignals(true);   // 阻止信号
    ui->pushButton->setChecked(checked);  // 更新 checked 状态
    // 根据状态选择对应的缓存图标
    ui->pushButton->setIcon(checked ? s_iconOn : s_iconOff);
    ui->pushButton->blockSignals(false);  // 恢复信号
}

/**
 * @brief 按钮点击槽函数
 * 
 * 当用户点击按钮时，Qt 自动调用此槽函数。
 * 函数名遵循 Qt 自动连接命名约定：on_<objectName>_<signal>
 * 
 * @param checked 按钮的新状态（由 Qt 传入）
 */
void appswitch::on_pushButton_clicked(bool checked)
{
    // 根据状态切换图标（使用缓存的静态图标，避免重复创建 QIcon 对象）
    // 三元运算符使代码更简洁
    ui->pushButton->setIcon(checked ? s_iconOn : s_iconOff);
    
    // 发射状态改变信号，携带索引和状态
    // 接收方可直接使用索引定位，时间复杂度 O(1)
    emit switchStateChanged(m_appIndex, checked);
}

/**
 * @brief 获取按钮指针
 * @return 返回内部 QPushButton 的指针
 * @note 添加 const 修饰符表示此方法不会修改对象状态
 */
QPushButton* appswitch::getPushButton() const
{
    return ui->pushButton;
}
