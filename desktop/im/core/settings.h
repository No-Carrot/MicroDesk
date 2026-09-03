//全局配置文件实例,读取配置文件信息, 查看 Conf 接口.
#ifndef SETTINGS_H
#define SETTINGS_H
#include "conf.h"
namespace fl_common {

class CORE_LIBRARY Settings : public Conf
{
public:
    static Settings *instance();

    //配置文件中键值对 key=KEY_V_UP;KEY_V_DOWN;KEY_V_HOME;KEY_V_ESC   valuetoList（"key"）
    QStringList valuetoList(const QString &value);

    explicit Settings();
};

}//end namespace
#endif // SETTINGS_H
