#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QPushButton>

class KeyboardWidgetPrivate;
class KeyboardWidget : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardWidget(QObject *parent = nullptr);
    ~ KeyboardWidget();
    void show();
    void hide();
    bool isVisible();
    void resize();
private:
    bool eventFilter(QObject *watched, QEvent *event) override;
    KeyboardWidgetPrivate *m_d;
};

class ButtonKey : public QPushButton{
public:
    ButtonKey(QWidget *parent=nullptr);
    void set(const QString &text, int key);
    int key();
    QString text();

private:
    QString m_text;
    QString m_showText;
    int m_key;
};

#endif // KEYBOARDWIDGET_H
