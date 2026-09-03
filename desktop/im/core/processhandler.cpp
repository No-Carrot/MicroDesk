#include "processhandler.h"
namespace fl_common {
ProcessHandler::~ProcessHandler()
{
    if(m_process != nullptr){
        this->stop();
        delete m_process;
        m_process = nullptr;
    }
}

void ProcessHandler::start(const QString &program)
{
    this->start(program, QStringList(), QProcess::ManagedInputChannel, QProcess::MergedChannels);
}

void ProcessHandler::start(const QString &program, const QStringList &arg)
{
    this->start(program, arg, QProcess::ManagedInputChannel, QProcess::MergedChannels);
}

void ProcessHandler::start(const QString &program, QStringList arg, QProcess::InputChannelMode inputMode, QProcess::ProcessChannelMode mode)
{
    if(m_process == nullptr){
        m_process = new QProcess;
        QObject::connect(m_process, &QProcess::readyRead, m_process, [this](){
            processMessage(m_process->readAll());
        });
    }
    m_process->setProcessChannelMode(mode);
    m_process->setInputChannelMode(inputMode);
    if(m_process->isOpen()) {
        this->stop();

    }
    arg.isEmpty() ? m_process->start(program) : m_process->start(program, arg);
}

void ProcessHandler::stop()
{
    if(m_process != nullptr && m_process->isOpen()){
        m_process->close();
        m_process->kill();
        m_process->waitForFinished();
    }
}

void ProcessHandler::send(const QByteArray &data)
{
    if(m_process->isOpen()) {
        m_process->write(data);
        m_process->waitForBytesWritten(1000);
    }
}

bool ProcessHandler::isStart()
{
    if(m_process)
        return m_process->isOpen();

    return false;
}
}
