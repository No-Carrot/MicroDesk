#ifndef IMPLATFORMINPUTCONTEXT_H
#define IMPLATFORMINPUTCONTEXT_H

#include <QtCore/qobject.h>
#include <qpa/qplatforminputcontext.h>
class IMInputContext;
class AbstractInputMethod;
class KeyboardWidget;
class IMPlatformInputContext : public QPlatformInputContext
{
    Q_OBJECT
public:
    explicit IMPlatformInputContext();
     ~IMPlatformInputContext();

    bool isValid() const override;
    void commit() override;
    virtual void showInputPanel() override;
    virtual void hideInputPanel() override;
    virtual bool isInputPanelVisible() const override;
    virtual void setFocusObject(QObject *object) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query);
    virtual void update(Qt::InputMethodQueries);

    QObject *focusObject();

    void changeInputMothod(AbstractInputMethod *input);
    bool isVisble();

Q_SIGNALS:
    void foucusObjectChange(QObject *object);

private:

    IMInputContext *m_inputContext;
    AbstractInputMethod *m_inputMethod;
    QObject *m_focusObject;
    KeyboardWidget *m_keyboard;
    bool m_bVisible;
};

#endif // IMPLATFORMINPUTCONTEXT_H
