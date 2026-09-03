#ifndef TERMINAL_H
#define TERMINAL_H

#include <QString>
#include "core_global.h"
namespace fl_common {
class CORE_LIBRARY  Terminal
{

public:
    /*!
     * \brief execCmd 执行非堵塞命令或者快速返回命令获取命令执行结果.(此函数会有堵塞时间，如果频繁调用导致UI卡顿请在线程中调用)
     * \param cmd example cat /home/elf/1.txt /ifconfig /ps -ef / ls ...
     * \return  命令执行结果 example 返回 1.txt内容 exec("cat /home/elf/1.txt")
     */
    static QString execCmd(const QString cmd, int waitMSeconds=-1);

    /*!
     * \brief killProcess杀死包含name 的所有进程
     * \param name 运行进程全名或者包含name 进程
     * \return  结果 killProcess("qtdemo_test")
     */
    static QString killProcess(const QString &name);

    /*!
     * \brief getMatch 从字符str中 安正则形式pattern 匹配所有的字符串. 例子 :解析data 所有IP地址 Terminal::getMatch(data, "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
     * \param str 要查找字符串。
     * \param pattern 匹配的正自表达式
     * \return    返回的匹配的字符串.
     */
    static QStringList getMatch(const QString &str, const QString &pattern);

};
}//end namespace
#endif // TERMINAL_H
