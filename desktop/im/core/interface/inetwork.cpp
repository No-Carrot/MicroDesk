#include "inetwork.h"
#include <QFile>
#include "terminal.h"
#include "conf.h"
#include  <QDebug>
#include <QThread>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfoList>

namespace fl_common {

//修改/etc/network/ineterface
class ConfNetworkInterface : public Conf
{
public:
    ConfNetworkInterface();
    void setAuto(const QString &cardName);
    void setStatic(const QString &cardName, const QString &ip, const QString &mask, const QString &gateway, const QString &dns);
    void readConf(const QString &cardName, QString &ip, QString &mask,  QString &gateway, QStringList &dns, bool &bAuto);

private:
    void createlo();
};

//修改/etc/systemd/network/*.network
class ConfSystemdNetwork
{
public:
    void setAuto(const QString &cardName);
    void setStatic(const QString &cardName, const QString &ip, const QString &gateway, const QString &dns);
    void readConf(const QString &cardName, QString &ip, QString &mask,  QString &gateway, QStringList &dns, bool & bAuto);

private:
    QString searchConf(const QString &cardname);
};

ConfNetworkInterface::ConfNetworkInterface():Conf("/etc/network/interfaces", " ", "auto")
{
}

void ConfNetworkInterface::setAuto(const QString &cardName)
{
    //设置网卡开机自动挂在.
    this->createlo();

    //配置cardName信息.
    QString sectionName =QString("auto %1").arg(cardName);
    UMap<UString, UString> maps;
    maps.clear();
    maps.insert("iface", QString("%1 inet dhcp").arg(cardName));
    setSection(sectionName, maps, true);
}

void ConfNetworkInterface::setStatic(const QString &cardName, const QString &ip, const QString &mask, const QString &gateway, const QString &dns)
{
    //设置网卡开机自动挂在.
    this->createlo();

    //配置cardName信息.
    QString sectionName =QString("auto %1").arg(cardName);
    UMap<UString, UString> soureMaps = getSection(sectionName);
    //如果之前时动态IP ，必须重写section，如果不是，不用重写section，只进行修改和添加。
    bool bOverride = soureMaps.value("iface").contains("dhcp");
    UMap<UString, UString> maps;
    maps.insert("iface", QString("%1 inet static").arg(cardName));
    maps.insert("address", ip);
    maps.insert("netmask", mask);
    maps.insert("gateway", gateway);
    maps.insert("broadcast", INetwork::broadCast(ip, mask));
    setSection(sectionName, maps, bOverride);

    //添加DNS 配置文件，对原有nameserver 不进行删除，只进行修改.
    Conf con("/etc/resolv.conf", " ", "nameserver");
    sectionName = QString("nameserver %1").arg(dns);
    con.setSection(sectionName,  UMap<UString, UString>(), false);
}

void ConfNetworkInterface::readConf(const QString &cardName, QString &ip, QString &mask, QString &gateway, QStringList &dns, bool &bAuto)
{
    //配置cardName信息.
    QString sectionName =QString("auto %1").arg(cardName);
    UMap<UString, UString> soureMaps = getSection(sectionName);
    UMap<UString, UString> maps= getSection(sectionName);

    ip = maps.value("address");
    mask = maps.value("netmask");
    gateway = maps.value("gateway");
    bAuto = maps.keys().contains(QString("%1 inet auto").arg(cardName));
    dns = INetwork::getdns();
}

void ConfNetworkInterface::createlo()
{
    QString sectionName = QString("auto lo");
    if(getSection(sectionName).isEmpty()) {
        UMap<UString, UString> maps;
        maps.insert("iface", "lo inet loopback");
        setSection(sectionName, maps);
    }
}

void ConfSystemdNetwork::setAuto(const QString &cardName)
{
    Conf con(searchConf(cardName), "=", "[Match]");
    QString sectionName = "[Match]";
    UMap<UString, UString> maps;
    maps.insert("Name", cardName);
    maps.insert("KernelCommandLine", "!root=/dev/nfs");
    maps.insert("[Network]", "");
    maps.insert("DHCP", "yes");
    con.setSection(sectionName, maps);
}

void ConfSystemdNetwork::setStatic(const QString &cardName, const QString &ip,  const QString &gateway, const QString &dns)
{
    Conf con(searchConf(cardName), "=", "[Match]");
    QString sectionName = "[Match]";

    UMap<UString, UString> soureMaps = con.getSection(sectionName);
    bool bOverride = soureMaps.keys().contains("DHCP");
    UMap<UString, UString> maps;
    maps.insert("Name", cardName);
    maps.insert("KernelCommandLine", "!root=/dev/nfs");
    maps.insert("[Network]", "");
    maps.insert("Address", ip);
    maps.insert("Gateway", gateway);
    maps.insert("DNS", dns);
    con.setSection(sectionName, maps, bOverride);
}

void ConfSystemdNetwork::readConf(const QString &cardName, QString &ip, QString &mask, QString &gateway, QStringList &dns, bool&  bAuto)
{
    Conf con(searchConf(cardName), "=", "[Match]");
    QString sectionName = "[Match]";
    UMap<UString, UString> maps = con.getSection(sectionName);
    gateway = maps.value("Gateway");
    dns = INetwork::getdns();
    bAuto = (maps.value("DHCP")== "yes");
    ip = maps.value("Address");

    QStringList list = ip.split("/");
    if(list.size() == 2){
        ip  = list[0];
        mask ="255.255.255.0";
    }
}


QString ConfSystemdNetwork::searchConf(const QString &cardname)
{
    QString fileName ;
    QString sectionName = "[Match]";
    if(!QFile::exists(fileName)) {
        QDir dir("/etc/systemd/network");
        QFileInfoList list = dir.entryInfoList(QStringList()<<"*.network", QDir::Files,  QDir::Name);
        foreach (QFileInfo info, list) {
            Conf con(info.absoluteFilePath(), "=", "[Match]");
            if(con.getSection(sectionName).value("Name") == cardname){
                fileName = info.absoluteFilePath();
                break;
            }
        }
    }

    if(fileName.isEmpty()){
        fileName = QString("/etc/systemd/network/10-%1.network").arg(cardname);
        if(!QFile::exists(fileName)){
            system(QString("touch %1").arg(fileName).toLocal8Bit().data());
        }
    }

    return fileName;
}

// interface ...
QString INetwork::broadCast(const QString &ip, const QString &mask)
{
    // ip & mask | ~mask
    QString ret;
    QStringList ipList = ip.split('.');
    QStringList maskList = mask.split('.');
    if(ipList.size() !=4 || maskList.size() !=4)
    {
        return ret;
    }
    for(int i=0; i<4; i++){
        int ipV = ipList[i].toInt();
        int maskV = maskList[i].toInt();
        ret += QString::number((ipV&maskV)|(~ maskV & 255));
        if(i<3){
            ret +='.';
        }
    }
    return ret;
}

QString INetwork::getMaskBits(const QString &mask)
{ //获取子网掩码 二进制 1的个数.也就是网络号.
    int k =0;
    QStringList  list =mask.split(".");
    for(int i=0, u=1;i<list.size(); i++, u=1){
        int n = list[i].toInt();
        for(; u<=n; u=u<<1 )
            if(u & n)
                k++;
    }
    return QString::number(k);
}

QStringList INetwork::availableCard()
{
    QStringList  cards;
    QString s = Terminal::execCmd("ifconfig -a | grep ens | grep -v grep");
    s +=Terminal::execCmd("ifconfig -a | grep eth | grep -v grep");
    cards = Terminal::getMatch(s, "(ens\\d+)|(eth\\d+)");

    return cards;
}


QString INetwork::gateway()
{
    QString gateway;
    QString result = Terminal::execCmd(QString("ip route"));

    QStringList list =getIPs(result);
    if(list.size() >=1){
        gateway = list[0];
    }
    return gateway;
}

void INetwork::getNetwork(const QString &cardName, QString &ip, QString &mask,  QString &broadCast,  QStringList &dns)
{
    QString result = Terminal::execCmd(QString("ifconfig %1").arg(cardName));

    QString key;
    bool bSaveIp = false;
    QString address;
    for(int i=0; i<result.size(); i++){
        QChar c = result.at(i);
        key +=c;
        if(c.isNumber() || (bSaveIp && c=='.')){
            bSaveIp = true;
            address +=c;
        }else{
            if(bSaveIp){
                key = key.toLower();
                //qDebug()<<key <<"##" <<address;
                if(key.contains("inet") && ip.isEmpty()){
                    ip = address;
                    if(ip.size() <7 || !ip.contains("."))
                        ip.clear();
                }else if(key.contains("mask") && mask.isEmpty()){
                    mask = address;
                }else if(key.contains("cast") &&broadCast.isEmpty()){
                    broadCast = address;
                }
                key.clear();
                address.clear();
            }
            bSaveIp = false;
        }
    }


    dns = getdns();
}

QStringList INetwork::getIPs(const QString &data)
{
    return Terminal::getMatch(data, "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
}

bool INetwork::isUp(const QString &cardName)
{
    return !Terminal::execCmd(QString("ifconfig | grep %1 | grep -v grep").arg(cardName)).isEmpty();
}

bool INetwork::setUp(const QString &cardName, bool bUp)
{
    QString text;
    bUp ? text ="up" : text = "down";
    QString cmd = QString("ifconfig %1 %2").arg(cardName).arg(text);
    Terminal::execCmd(cmd);

    return true;
}

bool INetwork::isLinkUp(const QString &cardName)
{
    QString cmd= QString("ip link show %1").arg(cardName);
    QString result =Terminal::execCmd(cmd).toUpper();

    return !result.contains("NO-CARRIER");
}

bool INetwork::isRoot()
{
   static QString result = Terminal::execCmd("who");
   static bool isRootUser = result.contains("root") ||result.isEmpty();
   return isRootUser;
}

QStringList INetwork::getdns()
{
    //get dns.
    QString result = Terminal::execCmd("cat /etc/resolv.conf");
    return getIPs(result);
}

void INetwork::processMessage(const QString &info)
{
    emit runLogChange(info);
}


INetwork::~INetwork()
{ 
}

void NetworkingServer::setStatic(const QString &cardName, const QString &ip, const QString &mask, const QString &gateway, const QString &dns)
{
    if(!isRoot())
        return;
    creatFile();

    ConfNetworkInterface conf;
    conf.setStatic(cardName, ip, mask, gateway, dns);
}

void NetworkingServer::setAuto(const QString &cardName)
{
    if(!isRoot())
        return;

    creatFile();

    ConfNetworkInterface conf;
    conf.setAuto(cardName);
}

bool NetworkingServer::exec(int *state)
{
    if(state){
        *state = 0;
    }

    if(!isRoot())
        return false;

    system("sync");
    Terminal::execCmd("/etc/init.d/networking restart");

    return isRunning();
}

void NetworkingServer::readConf(const QString &cardName, QString &ip, QString &mask, QString &gateway, QStringList &dns, bool &bAuto)
{
    ConfNetworkInterface conf;
    conf.readConf(cardName, ip,mask, gateway, dns, bAuto);
}


bool NetworkingServer::isRunning()
{
    QString process= Terminal::execCmd("ps");
    process += Terminal::execCmd("ps -ef");
    return process.contains("NetworkManager");
}

void NetworkingServer::creatFile()
{
   if(!QFile::exists("/etc/network/interfaces")){
      if(!QFile::exists("/etc/network")){
          system("mkdir -p /etc/network");
      }
      system("touch /etc/network/interfaces");
   }

   if(!QFile::exists("/etc/resolv.conf")){
       system("touch /etc/resolv.conf");
   }
}

void ManualMangeNetwork::setAuto(const QString &cardName)
{
    if(!isRoot())
        return;

    this->creatFile();

    stop();

    NetworkingServer::setAuto(cardName);
    //uchpc分配ip 地址.
    QString cmd = QString("udhcpc -i %1 &").arg(cardName);
    this->start(cmd);
    //    ::system(cmd.toLocal8Bit().constData());
}

void ManualMangeNetwork::setStatic(const QString &cardName, const QString &ip, const QString &mask, const QString &gateway, const QString &dns)
{
    if(!isRoot())
        return;

    this->creatFile();

    this->stop();

    //保存到配置文件.重启后生效
    NetworkingServer::setStatic(cardName, ip, mask, gateway, dns);

    //当前通过命令设置临时立即生效.
    Terminal::execCmd(QString("ifconfig %1 %2 netmask %3").arg(cardName).arg(ip).arg(mask));
    Terminal::execCmd(QString("route add default gateway %1").arg(gateway));
}

bool ManualMangeNetwork::exec(int *state)
{
    if(!isRoot())
        return false;

    system("sync");
    if(state){
        isStart() ? *state=1: *state=0;
    }

    return true;
}

void ManualMangeNetwork::readConf(const QString &cardName, QString &ip, QString &mask, QString &gateway, QStringList &dns, bool &bAuto)
{
    ConfNetworkInterface conf;
    conf.readConf(cardName, ip,mask, gateway, dns, bAuto);
}

bool SystemdNetworkServer::isRunning(const QString &serverName)
{
    bool bRun = false;
    QString result = Terminal::execCmd(QString("systemctl  -a | grep %1").arg(serverName));

    qDebug()<< result;
    bRun = result.contains("running");

    return bRun;
}

void SystemdNetworkServer::creatFile()
{
    if(!QFile::exists("/etc/systemd/network"))
    {
        system("mkdir -p /etc/systemd/network");
    }
}

void SystemdNetworkServer::setStatic(const QString &cardName, const QString &ip, const QString &mask, const QString &gateway, const QString &dns)
{
    if(!isRoot())
        return;

    ConfSystemdNetwork conf;
    QString ipNet= ip +"/" + getMaskBits(mask);//172.16.24.112/24
    conf.setStatic(cardName, ipNet, gateway, dns);

    if(!isRunning("systemd-resolved.service")){
        Conf con("/etc/resolv.conf", " ", "nameserver");
        QString sectionName = QString("nameserver %1").arg(dns);
        con.setSection(sectionName,  UMap<UString, UString>(), false);
    }
}

void SystemdNetworkServer::setAuto(const QString &cardname)
{
    if(!isRoot())
        return;

    creatFile();

    ConfSystemdNetwork conf;
    conf.setAuto(cardname);
}

bool SystemdNetworkServer::exec(int *state)
{
    if(!isRoot())
        return false;

    creatFile();

    if(state){
        *state = 0;
    }

    system("sync");
    Terminal::execCmd("sudo systemctl restart systemd-networkd");

    return isRunning("systemd-networkd.service");
}

void SystemdNetworkServer::readConf(const QString &cardName, QString &ip, QString &mask, QString &gateway, QStringList &dns, bool &bAuto)
{
    ConfSystemdNetwork conf;
    conf.readConf(cardName, ip,mask, gateway, dns, bAuto);
}
}
