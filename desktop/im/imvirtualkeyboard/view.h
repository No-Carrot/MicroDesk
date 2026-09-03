#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QQuickView>

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);
    void resize();
    virtual void create();
    virtual void show();
    virtual void hide();
    virtual  bool isVisible();

    bool eventFilter(QObject *obj, QEvent *event);

protected:
    virtual void move(int x, int y);
    virtual QRect rect();


private:
    QQuickView *m_view;
};

#endif // VIEW_H
