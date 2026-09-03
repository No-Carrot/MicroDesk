#ifndef WIDGETVIEW_H
#define WIDGETVIEW_H
#include <QObject>

class QWidget;
class WidgetView : public QObject
{
    Q_OBJECT
public:
    WidgetView(QObject *parent = nullptr);

    void create() ;
    void show() ;
    void hide() ;
    bool isVisible() ;

private:
    void resize();
    void move(int x, int y);
    QRect rect();
    QWidget *focusTopWidget() const;
    QWidget *m_widget;
};

#endif // WIDGETVIEW_H
