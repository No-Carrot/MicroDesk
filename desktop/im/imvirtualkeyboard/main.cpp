#include <qpa/qplatforminputcontextplugin_p.h>
#include "implatforminputcontext.h"
#include <QtCore/qobject.h>
#include <QtCore/QtDebug>

class IMPlugin : public QPlatformInputContextPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "im.json")

public:
    IMPlatformInputContext *create(const QString&, const QStringList&) override;
};

IMPlatformInputContext *IMPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    
    qDebug() << "[IM Plugin] create() called with system:" << system;

    // 修复 BUG: 原代码 system.compare(system, ...) 永远返回 0
    // 正确写法: system.compare("im", ...) 
    if (system.compare(QLatin1String("im"), Qt::CaseInsensitive) == 0) {
        qDebug() << "[IM Plugin] Creating IMPlatformInputContext";
        return new IMPlatformInputContext;
    }
    
    qDebug() << "[IM Plugin] System mismatch, not creating context";
    return nullptr;
}

#include "main.moc"
