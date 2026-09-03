/**
 * @file closebutton.h
 * @brief 自定义关闭按钮头文件
 * 
 * 该文件定义了一个现代风格的关闭按钮，特点：
 * - 圆形背景
 * - 细线条 X 图标
 * - 平滑的悬停和点击动画效果
 * - 与开关控件风格统一
 */

#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>

/**
 * @class CloseButton
 * @brief 现代风格关闭按钮
 * 
 * 使用 QPainter 绘制，不依赖图片资源。
 * 支持三种状态：正常、悬停、按下
 */
class CloseButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CloseButton(QWidget *parent = nullptr);
    
    /**
     * @brief 设置按钮尺寸
     * @param size 按钮的宽高（正方形）
     */
    void setButtonSize(int size);

protected:
    // 重写绘制事件，自定义绘制关闭按钮
    void paintEvent(QPaintEvent *event) override;
    
    // 重写鼠标事件，实现悬停效果
    // Qt 5 使用 QEvent*, Qt 6 使用 QEnterEvent*
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent *event) override;
#else
    void enterEvent(QEvent *event) override;
#endif
    void leaveEvent(QEvent *event) override;

private:
    bool m_hovered = false;     ///< 鼠标是否悬停
    int m_buttonSize = 50;      ///< 按钮尺寸
    
    // 配色方案
    QColor m_normalBgColor{240, 240, 240};      ///< 正常状态背景色（浅灰）
    QColor m_hoverBgColor{255, 100, 100};       ///< 悬停状态背景色（红色）
    QColor m_pressedBgColor{220, 80, 80};       ///< 按下状态背景色（深红）
    QColor m_normalIconColor{150, 150, 150};    ///< 正常状态图标色（灰色）
    QColor m_hoverIconColor{255, 255, 255};     ///< 悬停状态图标色（白色）
};

#endif // CLOSEBUTTON_H
