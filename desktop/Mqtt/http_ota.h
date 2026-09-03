#ifndef HTTP_OTA_H
#define HTTP_OTA_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

/**
 * ThingsBoard 3.6.4 HTTP OTA 模块
 * 参考 cankao/http_tb_ota.c 与 http_firmware_client.py
 * 
 * 工作流程：
 * 1. 固件信息通过 MQTT 订阅 v1/devices/me/attributes 获取（服务器主动下发）
 * 2. 收到固件信息后，调用 startDownload() 通过 HTTP 下载固件
 * 3. CRC32 校验
 * 4. 复制到程序目录并重启
 * 
 * Token 从 QSettings 读取（Tocken/tocken）
 * 固件下载到 desktop/download 目录
 */
class HttpOta : public QObject
{
    Q_OBJECT
public:
    explicit HttpOta(QObject *parent, QSettings *settings);

    /**
     * 开始下载固件（由 MQTT 收到固件属性后调用）
     * @param host 服务器主机，如 xthings.cloud
     * @param fwTitle 固件标题
     * @param fwVersion 固件版本
     * @param fwSize 固件大小（字节）
     * @param fwChecksum 固件校验和（十六进制字符串）
     * @param fwChecksumAlg 校验算法（如 crc32）
     * @param downloadDir 固件下载目录
     * @param appDir 程序所在目录
     * @param executableName 可执行文件名
     */
    void startDownload(const QString &host,
                       const QString &fwTitle,
                       const QString &fwVersion,
                       int fwSize,
                       const QString &fwChecksum,
                       const QString &fwChecksumAlg,
                       const QString &downloadDir,
                       const QString &appDir,
                       const QString &executableName);

signals:
    void progress(int percent);
    void statusMessage(const QString &msg);
    void finished(bool success, const QString &errorMessage);

private slots:
    void onDownloadProgress(qint64 received, qint64 total);
    void onDownloadFinished();
    void onDownloadReadyRead();

private:
    quint32 calcCrc32(const QByteArray &data);
    quint32 crc32ReverseBytes(quint32 value);
    bool verifyAndCopyThenRestart();

    QSettings *m_settings;
    QNetworkAccessManager *m_network;
    QNetworkReply *m_downloadReply;
    QFile *m_file;

    QString m_host;
    QString m_fwTitle;
    QString m_fwVersion;
    int m_fwSize;
    QString m_fwChecksum;
    QString m_fwChecksumAlg;
    QString m_downloadDir;
    QString m_appDir;
    QString m_executableName;
    QString m_filePath;
};

#endif // HTTP_OTA_H
