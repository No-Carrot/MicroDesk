/*进程读写接口，继承成此类，重写processMessage接口实现数据的读取
 */
#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H
#include "core_global.h"
#include <QProcess>

namespace fl_common {
class CORE_LIBRARY ProcessHandler{

public:
    virtual ~ProcessHandler();

    void start(const QString &program);

    void start(const QString &program,  const QStringList &arg);

    void start(const QString &program, QStringList arg,  QProcess::InputChannelMode inputMode=QProcess::ManagedInputChannel,
               QProcess::ProcessChannelMode mode=QProcess::MergedChannels);
    void stop();

    void send(const QByteArray &data);

    virtual void processMessage(const QString &info) =0;

    bool isStart();

private:
    QProcess *m_process = nullptr;
};

}
#define PROCESSHANDLER , protected fl_common::ProcessHandler
#endif // PROCESSHANDLER_H
