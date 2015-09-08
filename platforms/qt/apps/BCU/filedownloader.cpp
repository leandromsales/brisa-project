#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl url, QString filename, QObject *parent) :
    QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),this, SLOT (fileDownloaded(QNetworkReply*)));

    m_filename = filename;
    m_url = url;

    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();

    qDebug() << "empty" << m_DownloadedData.isEmpty();
    qDebug() << "null" << m_DownloadedData.isNull();

    QString url = m_url.toString();
    if (url.endsWith(".jpg", Qt::CaseInsensitive)) {
        QImage image = QImage::fromData(m_DownloadedData);
        QFile outFile("pics/" + m_filename + ".jpg");
        outFile.open(QIODevice::WriteOnly);
        image.save(&outFile, "JPEG");
        outFile.close();
        qDebug() << "get file" << m_filename << "jpg";
    } else if (url.endsWith(".png", Qt::CaseInsensitive)) {
        QImage image = QImage::fromData(m_DownloadedData);
        QFile outFile("pics/" + m_filename + ".png");
        outFile.open(QIODevice::WriteOnly);
        image.save(&outFile, "PNG");
        outFile.close();
        qDebug() << "get file" << m_filename << "png";
    } else if (url.endsWith(".compe", Qt::CaseInsensitive)) {
        QFile file("files/" + m_filename + ".compe");
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << m_DownloadedData;
        qDebug() << "get file" << m_filename << "compe";
    } else {
        qDebug() << "invalid extension";
    }

    emit ready();
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}
