#ifndef INPUTMETHODMANAGER_H
#define INPUTMETHODMANAGER_H

#include <QObject>
#include "abstractinputmethod.h"
class QSettings;
class InputMethodManager : public QObject
{
    Q_OBJECT
public:
    static InputMethodManager *instance();

    AbstractInputMethod *curentInputMethod();

    void setCurrentInputMethod(int lge);
    void changeLanguage();

    ~InputMethodManager();

protected:
    void create(int lge);
    explicit InputMethodManager(QObject *parent = nullptr);

private:
    AbstractInputMethod *m_input;
    QList<AbstractInputMethod*> m_inputList;
    QList<int> m_lges;
    QSettings *m_settings;
};

#endif // INPUTMETHODMANAGER_H
