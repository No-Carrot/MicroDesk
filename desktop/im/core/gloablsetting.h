#ifndef GLOABLSETTING_H
#define GLOABLSETTING_H
#include "conf.h"


class GloablSetting :public conf
{
public:
    GloablSetting();

    BaseFactorySetting(const QString &fileName, const QString &cut, const QString &matchKey=QString("")):Conf(fileName, cut, matchKey){}
    void loadData(){
        //读取配置参数.

        backs = getSections("[backlight]");
        if(backs.isEmpty()){
            backs<<UMap<UString, UString>();
        }

        spi = getSections("[spi]");
        if(spi.isEmpty()){
            spi<<UMap<UString, UString>();
        }
        sim4g = getSections("[4g]");
        if(sim4g.isEmpty()){
            sim4g<<UMap<UString, UString>();
        }

        adcs = getSections("[adc]");
        if(adcs.isEmpty()){
            adcs<<UMap<UString, UString>();
        }

        key = getSection("[key]");
        wifi = getSections("[wifi]");
        watchdog = getSection("[watchdog]");

    }

    QList<UMap<UString, UString> > backs;
    UMap<UString, UString> key;
    UMap<UString, UString> watchdog;
    QList<UMap<UString, UString> > sim4g;
    QList<UMap<UString, UString> > wifi;
    QList<UMap<UString, UString> > spi;
    QList<UMap<UString, UString> > adcs;

    static GloablSetting *instance();
};

#endif // GLOABLSETTING_H
