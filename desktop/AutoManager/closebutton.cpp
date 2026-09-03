/**
 * @file closebutton.cpp
 * @brief 自定义关闭按钮实现文件
 */

#include "closebutton.h"

CloseButton::CloseButton(QWidget *parent)
    : QPushButton(parent)
{
    // 设置固定尺寸
    setFixedSize(m_buttonSize, m_buttonSize);
    
    // 设置鼠标追踪和光标
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    
    // 设置无边框透明背景（绘制由 paintEvent 处理）
    setFlat(true);
    setAttribute(Qt::WA_Hover, true);
}

void CloseButton::setButtonSize(int size)
{
    m_buttonSize = size;
    setFixedSize(size, size);
    update();
}

void CloseButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);  // 抗锯齿
    
    // ============ 确定当前状态的颜色 ============
    QColor bgColor, iconColor;
    
    if (isDown()) {
        // 按下状态
        bgColor = m_pressedBgColor;
        iconColor = m_hoverIconColor;
    } else if (m_hovered) {
        // 悬停状态
        bgColor = m_hoverBgColor;
        iconColor = m_hoverIconColor;
    } else {
        // 正常状态
        bgColor = m_normalBgColor;
        iconColor = m_normalIconColor;
    }
    
    // ============ 绘制圆形背景 ============
    const int padding = 4;  // 内边距
    QRectF bgRect(padding, padding, 
                  width() - 2 * padding, 
                  height() - 2 * padding);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawEllipse(bgRect);  // 绘制圆形
    
    // ============ 绘制 X 图标 ============
    // 计算 X 的位置和大小
    const qreal centerX = width() / 2.0;
    const qreal centerY = height() / 2.0;
    const qreal crossSize = (m_buttonSize - 2 * padding) * 0.28;  // X 的半长度
    
    // 设置画笔：圆头线条，适中粗细
    QPen pen(iconColor);
    pen.setWidthF(2.5);
    pen.setCapStyle(Qt::RoundCap);  // 圆头
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    
    // 绘制两条交叉线形成 X
    painter.drawLine(QPointF(centerX - crossSize, centerY - crossSize),
                     QPointF(centerX + crossSize, centerY + crossSize));
    painter.drawLine(QPointF(centerX + crossSize, centerY - crossSize),
                     QPointF(centerX - crossSize, centerY + crossSize));
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void CloseButton::enterEvent(QEnterEvent *event)
#else
void CloseButton::enterEvent(QEvent *event)
#endif
{
    Q_UNUSED(event)
    m_hovered = true;
    update();  // 触发重绘
}

void CloseButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_hovered = false;
    update();  // 触发重绘
}
