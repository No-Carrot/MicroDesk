#include "terminal.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QElapsedTimer>
#include <QtDebug>

namespace fl_common {
QString Terminal::execCmd(const QString cmd, int waitMSeconds)
{ 
    QStringList arg;
    arg <<"-c"  << cmd;
    QString result;
    QProcess cmdExecer;
    cmdExecer.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
    cmdExecer.start("/bin/bash", arg);
    cmdExecer.waitForStarted();

    QElapsedTimer timer;
    if(waitMSeconds > 0){
        timer.start();
    }
    while(cmdExecer.waitForReadyRead()){
        result += cmdExecer.readAll();
        if(timer.elapsed() >= waitMSeconds)
            break;
    }

    return result;
}

QString Terminal::killProcess(const QString &name)
{
    QString ret;
    int index = -1;
    QStringList list = execCmd(QString("ps -ef | grep 'PID' | grep -v grep")).split(' ');

    for(int i=0; i<list.size(); i++){
        if(QString(list[i]).toUpper() =="PID"){
            index =i;
            break;
        }
    }

    if(index == -1){
        qDebug()<<QString("Not Process: %1").arg(name);
        return  "";
    }

    QString cmd = QString("ps -ef | grep '%1' | grep -v grep  | awk -F ' ' '{print $%2}'").arg(name).arg(index);
    QString pids = execCmd(cmd);
    list = pids.split("\n");
    foreach (QString pid, list) {
        if(!pid.isEmpty())
            ret =execCmd(QString("kill -9  %1").arg(pid));
    }

    return ret;
}

QStringList Terminal::getMatch(const QString &str, const QString &pattern)
{
    QStringList list;
    QRegularExpression re(pattern);
    int offset = 0;
    QRegularExpressionMatch match = re.match(str, offset);
    while (match.hasMatch()) {
        list <<match.captured();
        offset = match.capturedEnd();
        match = re.match(str, offset);
    }
    return list;
}
}
