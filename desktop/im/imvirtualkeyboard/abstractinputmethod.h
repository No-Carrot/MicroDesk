#ifndef ABSTRACTINPUTMETHOD_H
#define ABSTRACTINPUTMETHOD_H
#include <QObject>
#include <QMap>
class QKeyEvent;
class AbstractInputMethod: public QObject
{
    Q_OBJECT
public:
    virtual int language() =0;
    virtual bool keyEvent(QKeyEvent *)=0;

    virtual void commit(const QString &text=QString());
    virtual void reset();
    void commitText(const QString &text);
    void setPreeditText(const QString &text);
    void sendKeyEvent(QKeyEvent *e);

    static bool isNumber(int in);
    static  bool isLetter(int in);

    virtual QString getText(int key) ;

signals:
    void dataChange(const QStringList &data, const QString &pinyin);

private:
   QMap<int, QString> m_map;
};

class  DefaultInputMethod : public  AbstractInputMethod
{
    Q_OBJECT
public:
    virtual int language();
    bool keyEvent(QKeyEvent *e);
};


class  GroupInputMethod : public  AbstractInputMethod
{
    Q_OBJECT
public:
    virtual bool keyEvent(QKeyEvent *e);
    virtual bool handleKeyPressEvent(QKeyEvent *e) =0;
    virtual void reset();

private:
    int m_mode =0;
    bool m_bShitf=false;
};

#endif // ABSTRACTINPUTMETHOD_H
