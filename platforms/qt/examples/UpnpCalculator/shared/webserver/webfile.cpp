#include "webfile.h"

#include "webserversession.h"
#include "http/httpresponse.h"
#include <QScopedPointer>

namespace brisa {
namespace shared {
namespace webserver {

using namespace http;

WebFile::WebFile(const QString &fileName, QObject *parent) :
        WebService(parent),
        m_fileName(fileName),
        m_useChunkedEntities(false) {
    qDebug() << "INSTANCIOU WEB FILE";
}

WebFile::~WebFile()
{
}

void WebFile::setFile(const QString &fileName)
{
    m_fileName = fileName;
}

void WebFile::setContentType(const QByteArray &cT)
{
    m_contentType = cT;
}

void WebFile::setUseChunkedEntities(bool u)
{
    m_useChunkedEntities = u;
}

void WebFile::onRequest(const HttpRequest &request,
                             WebserverSession *session)
{
    HttpResponse response(request.httpVersion());

    QScopedPointer<QFile> file(new QFile(m_fileName));
    if(!file->open(QIODevice::ReadOnly)){
    	qDebug() << "ERROR:WebFile::onRequest: The tmp file could not be opened. File path: " << m_fileName;
    }

    const qint64 fileSize = file->size();

    if (!m_contentType.isEmpty())
        response.setHeader("CONTENT-TYPE", m_contentType);

    if (request.header("ACCEPT-RANGES") == "bytes")
        response.setHeader("ACCEPT-RANGES", "bytes");

    if (!request.header("RANGE").isEmpty()) {
        QByteArray rangeHeader = request.header("RANGE");

        // from begin of range to the end of the file
        if (rangeHeader.indexOf('-') == -1) {
            // 6 = QByteArray("bytes=").size()
            QByteArray firstBytePos = rangeHeader
                                      .mid(rangeHeader.indexOf("bytes=") + 6);

            bool ok;
            qlonglong firstByte = firstBytePos.toLongLong(&ok);

            if (ok) {
                response.setRange(QPair<qlonglong, qlonglong>(firstByte,
                                                              fileSize - 1));
                response.setStatusCode(HttpResponse::PARTIAL_CONTENT);
            }
        } else {
            QByteArray firstBytePos = rangeHeader
                                      .mid(rangeHeader.indexOf("bytes=") + 6,
                                           rangeHeader.indexOf('-') - 6);
            QByteArray lastBytePos = rangeHeader.mid(rangeHeader.indexOf('-') + 1);

            // has initial bytePos
            if (!firstBytePos.isEmpty()) {
                bool ok[2];
                qlonglong firstByte = firstBytePos.toLongLong(ok);
                qlonglong lastByte = lastBytePos.toLongLong(ok + 1);

                if (ok[0]) {
                    if (ok[1]) {
                        if (firstByte < lastByte) {
                            response.setRange(QPair<qlonglong, qlonglong>(firstByte,
                                                                          lastByte));
                            response.setStatusCode(HttpResponse::PARTIAL_CONTENT);
                        }
                    } else {
                        response.setRange(QPair<qlonglong, qlonglong>(firstByte,
                                                                      fileSize - 1));
                    }
                }
            } else {
                bool ok;
                qlonglong lastBytes = lastBytePos.toLongLong(&ok);

                if (ok) {
                    response.setRange(QPair<qlonglong, qlonglong>(fileSize - lastBytes,
                                                                  fileSize - 1));
                    response.setStatusCode(HttpResponse::PARTIAL_CONTENT);
                }
            }
        }
    }

    response.setEntityBody(file.take());

    session->respond(response, m_useChunkedEntities);
}

}  // namespace webserver
}  // namespace shared
}  // namespace brisa
