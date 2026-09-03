#include "plughandler.h"
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDir>
#include "abstractinputmethod.h"
#include <QtDebug>


static PlugHandler *s_instance = NULL;
class PlugHandlerPrivate
{
public:
    QList<int> language;
    QList<AbstractInputMethod *> inputMethods;
    QList<QPluginLoader*> loaders;
};

PlugHandler::~PlugHandler()
{
    delete m_d;
}

PlugHandler *PlugHandler::instance()
{
    if(s_instance == NULL){
        s_instance = new  PlugHandler;
    }
    return s_instance;
}

AbstractInputMethod *PlugHandler::loadPlugins(int id)
{

    if(m_d->language.contains(id))
    {
        return   m_d->inputMethods.value(m_d->language.indexOf(id));
    }

    QDir pluginsDir = QDir(qApp->applicationDirPath()+"/../plugins");
    qDebug()<<"count "<< pluginsDir.entryInfoList(QDir::Files).size();

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {

        QPluginLoader *loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
        if(loader->load()){
            AbstractInputMethod *p = (AbstractInputMethod*)loader->instance();
            if(p->language() == id){
                m_d->language.push_back(id);
                m_d->inputMethods.push_back(p);
                m_d->loaders.push_back(loader);
                return p;
        }
        loader->unload();
        delete loader;
    }

    return NULL;
}

bool PlugHandler::unloadPlugins(int id)
{
    bool bResult = false;
      if(m_d->language.contains(id))
      {
          int index = m_d->language.indexOf(id);
          QPluginLoader *loader = m_d->loaders.value(id);
          if(loader && loader->unload()){
              m_d->inputMethods.removeAt(index);
              m_d->loaders.removeAt(index);
              m_d->language.removeAt(index);
          }
      }

      return bResult;
}
