#ifndef GCONFIG_H
#define GCONFIG_H

#include <QString>


// 加线程安全（C++11及以上版本的局部静态变量已经是线程安全的）
// 全局配置类声明
// 提供全局配置实例的便捷访问方式
#define GCInstance  (GConfig::Instance())


// 全局配置类
// 该类用于存储全局配置信息
class GConfig
{
public:
    qint32 screenWidth = 800; // 屏幕宽度
    qint32 screenHeight = 480; // 屏幕高度
    qint32 appIconRows = 2; // 桌面显示图标的行数
    qint32 appIconCols = 4; // 桌面显示图标的列数


    // 获取全局配置实例  
    // 该方法用于获取全局配置实例
    // @return GConfig& 全局配置实例
    // @note 该方法为静态方法，可直接通过类名调用
    static GConfig &Instance() // 获取单例实例
    {
        static GConfig instance; // 静态局部变量，线程安全
        return instance;
    }

private:
    // 构造函数私有化，防止外部实例化
    GConfig(){} // 构造函数私有化，防止外部实例化

};


#endif // GCONFIG_H
