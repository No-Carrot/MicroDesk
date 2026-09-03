#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLabel>
#include "ui_global.h"

class TitleBarPrivate;
class UI_EXPORT TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(const QString &title=QString(), bool bFullscreen =false, QWidget *parent = nullptr);

    ~TitleBar();

    virtual void setText(const QString &title);

Q_SIGNALS:
    void quit();

private:
    TitleBarPrivate *m_d;
};

#endif // TITLEBAR_H
