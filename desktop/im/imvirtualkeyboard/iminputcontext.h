#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <QObject>
class IMPlatformInputContext;
class IMInputContext : public QObject
{
    Q_OBJECT
public:
    explicit IMInputContext(QObject *parent=NULL);
    static IMInputContext *instance();

    bool eventFilter(QObject *watched, QEvent *event);

    IMPlatformInputContext *platformCtx();
    void setPlatformCtx(IMPlatformInputContext *ctx);

    bool  isInputPanelVisible();
    void setInputPanelVisible(bool bVisible);
    QObject *focusObject();
    void commit();


Q_SIGNALS:
    void dataChange(const QStringList &model, const QString &spellString);

private:
    IMPlatformInputContext *m_ctx;
    bool m_bVisible;
};

#endif // INPUTHANDLER_H
