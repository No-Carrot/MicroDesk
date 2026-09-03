#include "http_ota.h"
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>

// OTA 重启退出码，MicroDesktop 检测到此退出码后自动重启程序
#define OTA_RESTART_EXIT_CODE 100

HttpOta::HttpOta(QObject *parent, QSettings *settings)
    : QObject(parent)
    , m_settings(settings)
    , m_network(nullptr)
    , m_downloadReply(nullptr)
    , m_file(nullptr)
    , m_fwSize(0)
{
    m_network = new QNetworkAccessManager(this);
}

void HttpOta::startDownload(const QString &host,
                            const QString &fwTitle,
                            const QString &fwVersion,
                            int fwSize,
                            const QString &fwChecksum,
                            const QString &fwChecksumAlg,
                            const QString &downloadDir,
                            const QString &appDir,
                            const QString &executableName)
{
    // 检查参数
    if (!m_settings)
    {
        emit statusMessage(tr("OTA: 配置不可用"));
        emit finished(false, tr("配置不可用"));
        return;
    }
    QString token = m_settings->value("Tocken/tocken").toString();
    if (token.isEmpty())
    {
        emit statusMessage(tr("OTA: Token 为空"));
        emit finished(false, tr("Token 未配置"));
        return;
    }
    if (host.isEmpty() || fwTitle.isEmpty() || fwVersion.isEmpty() || fwSize <= 0)
    {
        emit statusMessage(tr("OTA: 固件信息不完整"));
        emit finished(false, tr("固件信息不完整"));
        return;
    }

    // 保存参数
    m_host = host;
    m_fwTitle = fwTitle;
    m_fwVersion = fwVersion;
    m_fwSize = fwSize;
    m_fwChecksum = fwChecksum;
    m_fwChecksumAlg = fwChecksumAlg;
    m_downloadDir = downloadDir;
    m_appDir = appDir;
    m_executableName = executableName;

    qDebug() << "OTA: Starting download";
    qDebug() << "  host:" << m_host;
    qDebug() << "  fw_title:" << m_fwTitle;
    qDebug() << "  fw_version:" << m_fwVersion;
    qDebug() << "  fw_size:" << m_fwSize;
    qDebug() << "  fw_checksum:" << m_fwChecksum;
    qDebug() << "  fw_checksum_algorithm:" << m_fwChecksumAlg;

    // 创建下载目录
    if (!QDir().mkpath(m_downloadDir))
    {
        emit statusMessage(tr("OTA: 无法创建下载目录"));
        emit finished(false, tr("无法创建下载目录"));
        return;
    }

    // 构建固件下载 URL
    // ThingsBoard 3.6.4: GET /api/v1/{token}/firmware?title=...&version=...
    QString urlStr = QString("http://%1/api/v1/%2/firmware?title=%3&version=%4")
                         .arg(m_host)
                         .arg(token)
                         .arg(QString::fromUtf8(QUrl::toPercentEncoding(m_fwTitle)))
                         .arg(QString::fromUtf8(QUrl::toPercentEncoding(m_fwVersion)));
    QUrl url(urlStr);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mqtt-TB-OTA/1.0");

    qDebug() << "OTA: Download URL:" << urlStr;

    // 准备文件
    m_filePath = QDir(m_downloadDir).absoluteFilePath("firmware.bin");
    if (m_file)
    {
        m_file->close();
        delete m_file;
    }
    m_file = new QFile(m_filePath);
    if (!m_file->open(QIODevice::WriteOnly))
    {
        qDebug() << "OTA: Cannot open file" << m_filePath;
        emit statusMessage(tr("OTA: 无法创建文件"));
        emit finished(false, tr("无法创建文件"));
        delete m_file;
        m_file = nullptr;
        return;
    }

    // 开始下载
    if (m_downloadReply)
    {
        m_downloadReply->abort();
        m_downloadReply->deleteLater();
    }
    m_downloadReply = m_network->get(req);
    connect(m_downloadReply, &QNetworkReply::downloadProgress, this, &HttpOta::onDownloadProgress);
    connect(m_downloadReply, &QNetworkReply::finished, this, &HttpOta::onDownloadFinished);
    connect(m_downloadReply, &QNetworkReply::readyRead, this, &HttpOta::onDownloadReadyRead);

    emit statusMessage(tr("OTA: 下载中..."));
    emit progress(0);
}

void HttpOta::onDownloadReadyRead()
{
    if (m_downloadReply && m_file && m_file->isOpen())
    {
        m_file->write(m_downloadReply->readAll());
    }
}

void HttpOta::onDownloadProgress(qint64 received, qint64 total)
{
    int percent = 0;
    if (total > 0)
        percent = static_cast<int>((received * 100) / total);
    else if (m_fwSize > 0)
        percent = static_cast<int>((received * 100) / m_fwSize);
    emit progress(percent);
}

void HttpOta::onDownloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply || reply != m_downloadReply)
        return;

    m_downloadReply = nullptr;

    // 写入剩余数据并关闭文件
    if (m_file && m_file->isOpen())
    {
        m_file->write(reply->readAll());
        m_file->close();
    }

    // 检查下载错误
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "OTA: Download error:" << reply->errorString();
        emit statusMessage(tr("OTA: 下载失败"));
        if (m_file)
        {
            m_file->remove();
            delete m_file;
            m_file = nullptr;
        }
        reply->deleteLater();
        emit finished(false, reply->errorString());
        return;
    }
    reply->deleteLater();

    emit progress(100);
    emit statusMessage(tr("OTA: 校验并安装..."));

    if (m_file)
    {
        delete m_file;
        m_file = nullptr;
    }

    // 校验并复制
    if (verifyAndCopyThenRestart())
    {
        emit finished(true, QString());
    }
    else
    {
        emit statusMessage(tr("OTA: 校验失败"));
        emit finished(false, tr("CRC32 校验失败"));
    }
}

quint32 HttpOta::calcCrc32(const QByteArray &data)
{
    // CRC32 查找表（与参考代码一致）
    static quint32 table[256];
    static bool tableOk = false;
    if (!tableOk)
    {
        const quint32 poly = 0xEDB88320u;
        for (quint32 i = 0; i < 256; i++)
        {
            quint32 c = i;
            for (int j = 0; j < 8; j++)
                c = (c & 1) ? ((c >> 1) ^ poly) : (c >> 1);
            table[i] = c;
        }
        tableOk = true;
    }

    quint32 crc = 0xFFFFFFFFu;
    for (int i = 0; i < data.size(); i++)
        crc = (crc >> 8) ^ table[(crc ^ static_cast<quint8>(data.at(i))) & 0xFF];
    return crc ^ 0xFFFFFFFFu;
}

quint32 HttpOta::crc32ReverseBytes(quint32 value)
{
    // 字节序反转（与参考代码一致）
    return ((value & 0x000000FFu) << 24) |
           ((value & 0x0000FF00u) << 8) |
           ((value & 0x00FF0000u) >> 8) |
           ((value & 0xFF000000u) >> 24);
}

bool HttpOta::verifyAndCopyThenRestart()
{
    // 读取下载的固件文件
    QFile f(m_filePath);
    if (!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "OTA: Cannot open for verify:" << m_filePath;
        return false;
    }
    QByteArray data = f.readAll();
    f.close();

    // 检查文件大小
    if (data.size() != m_fwSize)
    {
        qDebug() << "OTA: Size mismatch, got" << data.size() << "expected" << m_fwSize;
        return false;
    }

    // CRC32 校验
    if (m_fwChecksumAlg.compare("crc32", Qt::CaseInsensitive) != 0)
    {
        qDebug() << "OTA: Unsupported checksum algorithm:" << m_fwChecksumAlg;
        return false;
    }

    quint32 calculated = calcCrc32(data);
    quint32 reversed = crc32ReverseBytes(calculated);
    bool ok = false;
    quint32 expected = m_fwChecksum.trimmed().toUInt(&ok, 16);
    if (!ok)
    {
        qDebug() << "OTA: Invalid checksum string:" << m_fwChecksum;
        return false;
    }

    qDebug() << "OTA: Checksum verification:";
    qDebug() << "  Expected:" << Qt::hex << expected;
    qDebug() << "  Calculated:" << Qt::hex << calculated;
    qDebug() << "  Reversed:" << Qt::hex << reversed;

    // 比较校验和（支持正常和字节序反转两种情况）
    if (reversed != expected && calculated != expected)
    {
        qDebug() << "OTA: CRC32 mismatch!";
        return false;
    }

    qDebug() << "OTA: Checksum verification passed!";

    // 复制固件到程序目录
    QString newPath = m_appDir + "/" + m_executableName + ".new";
    QString bakPath = m_appDir + "/" + m_executableName + ".bak";
    QString exePath = m_appDir + "/" + m_executableName;
    
    QFile newFile(newPath);
    if (newFile.exists())
        newFile.remove();
    if (!QFile::copy(m_filePath, newPath))
    {
        qDebug() << "OTA: Copy failed to" << newPath;
        return false;
    }

    // 设置可执行权限
    QFile::setPermissions(newPath,
                          QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
                          QFile::ReadGroup | QFile::ExeGroup |
                          QFile::ReadOther | QFile::ExeOther);

    qDebug() << "OTA: Firmware copied to" << newPath;

    // 执行文件替换
    QFile::remove(bakPath);           // 删除旧备份
    QFile::rename(exePath, bakPath);  // 备份当前程序
    QFile::rename(newPath, exePath);  // 替换为新程序

    qDebug() << "OTA: Firmware replaced successfully";
    qDebug() << "OTA: Will exit with code" << OTA_RESTART_EXIT_CODE << "for MicroDesktop to restart";
    
    return true;
}
