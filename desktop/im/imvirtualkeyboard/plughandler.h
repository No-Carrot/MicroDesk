#ifndef PLUGHANDLER_H
#define PLUGHANDLER_H
#include <QObject>
class PlugHandlerPrivate;
class AbstractInputMethod;
class PlugHandler
{
public:
    ~PlugHandler();

    static PlugHandler* instance();

    AbstractInputMethod* loadPlugins(int id);
    bool unloadPlugins(int id);

private:
    PlugHandlerPrivate *m_d;
};

#endif // PLUGHANDLER_H
