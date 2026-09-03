#include "ibase.h"
#include <iostream>
#include <unistd.h>
#include "datareload.h"

namespace fl_common {
IBase::IBase() :m_fd(-1)
{
}

IBase::~IBase()
{
    close();
    if(m_d)
        delete m_d;
}

bool IBase::open()
{
    return  m_fd;
}

void IBase::close()
{
    if(m_fd > 0)
    {
        ::close(m_fd);
        m_fd = -1;
    }
}

QString IBase::devName()
{
    return m_d->data().value("devName");
}

void IBase::loadData(DataReload *d)
{
    m_d = d;
}

int IBase::id()
{
    return m_d->id();
}

QString IBase::name()
{
   return m_d->data().value("name");
}


int IBase::handle()
{
    return m_fd;
}

bool IBase::isOpen()
{
    return m_fd > 0;
}
}
