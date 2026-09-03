#include "basefactory.h"
#include <QGlobalStatic>
#include "conf.h"
#include "datareload.h"
#include "terminal.h"
#include <QFile>
#include <QGlobalStatic>
#include <QDebug>
#include "basepath.h"
#include "settings.h"
#include "dispbacklight.h"

namespace fl_common {

Q_GLOBAL_STATIC(BaseFactory, s_instance)

class  FactorySetting{
public:
    void loadData(){
        //读取配置参数.
        Settings *pSet= Settings::instance();
        backs =  pSet->getSections("[backlight]");
        if(backs.isEmpty()){
            backs<<UMap<UString, UString>();
        }

        spi = pSet->getSections("[spi]");
        if(spi.isEmpty()){
            spi<<UMap<UString, UString>();
        }
        sim4g = pSet->getSections("[4g]");
        if(sim4g.isEmpty()){
            sim4g<<UMap<UString, UString>();
        }

        adcs = pSet->getSections("[adc]");
        if(adcs.isEmpty()){
            adcs<<UMap<UString, UString>();
        }

        key = pSet->getSection("[key]");
        wifi = pSet->getSections("[wifi]");
        watchdog = pSet->getSection("[watchdog]");
    }

    QList<UMap<UString, UString> > backs;
    UMap<UString, UString> key;
    UMap<UString, UString> watchdog;
    QList<UMap<UString, UString> > sim4g;
    QList<UMap<UString, UString> > wifi;
    QList<UMap<UString, UString> > spi;
    QList<UMap<UString, UString> > adcs;
};

class BaseFactoryPrivate{
public:
    QMap<int, IBacklight*> backlights;
    QMap<int, ISpi*> spis;
    QMap<int, ISIMNetwork*> sims;
    QMap<int, IAdc*> adcs;

    IKey * key = nullptr;
    INetwork *network = nullptr;
    IWatchDog *watchdog = nullptr;
    QList<IBase*> bases;

    //配置文件静态变量.
    FactorySetting *setting = NULL;

    DataReload * create(int id, const UMap<UString, UString> &d){
        DataReload *data = new DataReload(id, d);
        return data;
    }
};

BaseFactory *BaseFactory::instance()
{
    return s_instance;
}

BaseFactory::BaseFactory()
{
    m_d =new BaseFactoryPrivate;

    if(m_d->setting == nullptr){
        m_d->setting = new FactorySetting;
    }
    //加载配置数据.
    m_d->setting->loadData();
}

BaseFactory::~BaseFactory()
{
    foreach (IBase *b, m_d->bases) {
        if(b->isOpen())
            b->close();
        delete b;
    }
    delete m_d;
}

IKey *BaseFactory::key()
{
    if(m_d->key == nullptr){
        m_d->key = new IKey;
        m_d->key->loadData(m_d->create(0, m_d->setting->key));
        m_d->bases << m_d->key;
    }
    return m_d->key;
}

IWatchDog *BaseFactory::watchdog()
{
    if(m_d->watchdog == nullptr) {
        m_d->watchdog = new IWatchDog;
        if(m_d->setting->watchdog.isEmpty())
        {
            m_d->setting->watchdog.insert("devName","/dev/watchdog");
        }
        m_d->watchdog->loadData(m_d->create(0, m_d->setting->watchdog));
        m_d->bases << m_d->watchdog;
    }
    return m_d->watchdog;
}

INetwork *BaseFactory::wriednetwork()
{
    if(m_d->network == nullptr){
        QString systemd_networkd=Terminal::execCmd("systemctl status systemd-networkd | grep running");
        bool bSystemCtl = systemd_networkd.contains("running");
        if(bSystemCtl){
            m_d->network = new SystemdNetworkServer;
        }else if(QFile::exists("/etc/init.d/networking")){
            m_d->network = new NetworkingServer;
        }else {
            m_d->network = new ManualMangeNetwork;
        }
        m_d->bases << m_d->network;
    }
    return m_d->network;
}

int BaseFactory::backlightCount()
{
    if(m_d->backlights.isEmpty())
        backlight(0);

    return m_d->backlights.count();
}

IBacklight *BaseFactory::backlight(int id)
{
    if(m_d->backlights.isEmpty()){

        for(int i=0; i<m_d->setting->backs.count(); i++){
            DataReload * data= m_d->create(i, m_d->setting->backs[i]);
            IBacklight *bk = new IBacklight;
            if(data->data().value("type", "0") == "1"){
                bk = new DispBacklight;
            }else{
                bk = new IBacklight;
            }
            bk->loadData(data);
            m_d->backlights.insert(i, bk);
            m_d->bases <<bk;
        }
    }

    if(id >= m_d->backlights.size() || id <0)
        id = 0;

    return m_d->backlights[id];
}

int BaseFactory::simNetworkCount()
{
    if(m_d->sims.isEmpty())
        simNetowrk(0);
    return m_d->sims.count();
}


ISIMNetwork *BaseFactory::simNetowrk(int id)
{
    if(m_d->sims.isEmpty()) {
        for(int i=0; i<m_d->setting->sim4g.count(); i++)
        {
            ISIMNetwork *network = new ISIMNetwork;
            network->loadData(m_d->create(i, m_d->setting->sim4g[i]));
            m_d->sims.insert(i, network);
            m_d->bases << network;
        }
    }

    return m_d->sims[id];
}

int BaseFactory::spiCount()
{
    if(m_d->spis.isEmpty()){
        spi(0);
    }
    return m_d->spis.count();
}

ISpi *BaseFactory::spi(int id)
{
    if(m_d->spis.isEmpty()) {
        for(int i=0; i<m_d->setting->spi.size(); i++){
            DataReload * data= m_d->create(i, m_d->setting->spi[i]);
            ISpi *spi = new ISpi;
            spi->loadData(data);
            m_d->spis.insert(i, spi);
            m_d->bases << spi;
        }
    }

    if(id >= m_d->spis.size() || id <0)
        id = 0;

    return m_d->spis[id];
}

int BaseFactory::adcCount()
{
    if(m_d->adcs.isEmpty())
        adc(0);
    return m_d->adcs.count();
}

IAdc *BaseFactory::adc(int id)
{
    if(m_d->adcs.isEmpty()){
        for(int i=0; i<m_d->setting->adcs.size(); i++){
            DataReload * data= m_d->create(i, m_d->setting->adcs[i]);
            IAdc *pAdc = new IAdc;
            pAdc->loadData(data);
            m_d->adcs.insert(i, pAdc);
            m_d->bases <<pAdc;
        }
    }

    if(id >= m_d->adcs.size() || id <0)
        id = 0;

    return m_d->adcs[id];
}

}//end namespace
