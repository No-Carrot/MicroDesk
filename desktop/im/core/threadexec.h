#ifndef THREADEXEC_H
#define THREADEXEC_H

#include <QRunnable>
#include <QGlobalStatic>
#include <QThreadPool>
#include <functional>

namespace fl_common {
class ThreadExec : public QObject
{
    Q_OBJECT
public:
    template<typename T>
    class Runable: public QRunnable{
    public:
        Runable( ThreadExec *_q, std::function<void(T)> f,  T t):q(_q),m_fn(f),m_t(t){}
        void run(){
            m_fn(m_t);
            q->finish();
            q->deleteLater();
        }

    public:
        std::function<void(T)> m_fn;
        ThreadExec *q;
        T m_t;
    };

    template<typename T>
    void start(std::function<void(T)> f, T t){
        Runable<T> *run = new Runable<T>( this, f, t);
        run->setAutoDelete(true);
        QThreadPool::globalInstance()->start(run);
    }

Q_SIGNALS:
    void finish();
};
}

#endif // THREADEXEC_H
