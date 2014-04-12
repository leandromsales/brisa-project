#include "webserversession.h"
#include "webserver.h"
#include "webservice.h"

#include <QTimer>

#ifndef MAPPED_MEMORY_SIZE
#define MAPPED_MEMORY_SIZE 64
#endif

// in miliseconds
#ifndef SESSION_TIMEOUT
#define SESSION_TIMEOUT 180000
#endif

namespace brisa {
namespace shared {
namespace webserver {

using namespace http;

enum State
{
    WAITING_FOR_CHUNK_SIZE = 1,
    WAITING_FOR_CHUNK,
    WAITING_FOR_TRAILLING_HEADERS
};

WebserverSession::WebserverSession(Webserver *server, HttpSessionManager *parent) :
    HttpSession(parent),
    server(server),
    timer(new QTimer)
{
    lastSupportedHttpVersion = HttpVersion(1, 1);
    connect(this, SIGNAL(responsePosted(HttpResponse)), this, SLOT(writeResponse(HttpResponse)));
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

WebserverSession::~WebserverSession()
{
    delete timer;
}

void WebserverSession::respond(HttpResponse r, bool chunkedResponse)
{
    useChunkedResponse = chunkedResponse;
    emit responsePosted(r);
}

int WebserverSession::isRequestSupported(const HttpRequest &request) const
{
    if ((request.httpVersion().major() != 1)
        || (request.httpVersion().minor() != 0 && request.httpVersion().minor() != 1)) {
        return HttpResponse::HTTP_VERSION_NOT_SUPPORTED;
    }

    if (request.method() != "GET" && request.method() != "POST"
        && request.method() != "SUBSCRIBE" && request.method() != "UNSUBSCRIBE"
            && request.method() != "NOTIFY") {
        return HttpResponse::NOT_IMPLEMENTED;
    }
    return 0;
}

bool WebserverSession::hasEntityBody(const HttpRequest &request) throw(HttpResponse)
{
    if (request.method() == "POST") {
        // REQUIRED. Field value MUST be text/xml; charset="utf-8" for description documents.
        if (request.header("CONTENT-TYPE").isNull()) {
            throw HttpResponse(request.httpVersion(), HttpResponse::BAD_REQUEST);
        }/* else if(request.header("CONTENT-TYPE") != "text/xml; charset=\"utf-8\"") {
            throw HttpResponse(request.httpVersion(), HttpResponse::BAD_REQUEST);
        }*/

        if (!request.header("CONTENT-LENGTH").isNull()) {
            bool ok;
            entitySize = request.header("CONTENT-LENGTH").toInt(&ok);

            if (!ok)
                throw HttpResponse(request.httpVersion(), HttpResponse::BAD_REQUEST);

            chunkedEntity = false;
        } else if (request.header("TRANSFER-ENCODING") == "chunked") {
            chunkedEntity = true;
        } else {
            throw HttpResponse(request.httpVersion(), HttpResponse::LENGTH_REQUIRED);
        }

        return true;

    } else if (request.method() == "NOTIFY"){
        if (request.header("CONTENT-TYPE").isNull()) {
            throw HttpResponse(request.httpVersion(), HttpResponse::BAD_REQUEST);
        }

        if (!request.header("CONTENT-LENGTH").isNull()) {
            bool ok;
            entitySize = request.header("CONTENT-LENGTH").toInt(&ok);

            if (!ok)
                throw HttpResponse(request.httpVersion(), HttpResponse::BAD_REQUEST);

            chunkedEntity = false;
        } else if (request.header("TRANSFER-ENCODING") == "chunked") {
            chunkedEntity = true;
        } else {
            throw HttpResponse(request.httpVersion(), HttpResponse::LENGTH_REQUIRED);
        }

        return true;
    } else {
        return false;
    }
}

bool WebserverSession::atEnd(HttpRequest &request, QByteArray &buffer) throw(HttpResponse)
{
    while (true) {
        switch (chunkedEntity) {
        case WAITING_FOR_CHUNK_SIZE:
            {
                int i = buffer.indexOf("\r\n");
                if (i != -1) {
                    QByteArray n = buffer.left(i);
                    buffer.remove(0, i + 2);

                    bool ok;
                    entitySize = n.toInt(&ok, 16);

                    if (!ok)
                        throw HttpResponse(request.httpVersion(), HttpResponse::BAD_REQUEST, true);

                    if (entitySize)
                        chunkedEntity = WAITING_FOR_CHUNK;
                    else
                        chunkedEntity = WAITING_FOR_TRAILLING_HEADERS;

                    break;
                } else {
                    return false;
                }
            }
        case WAITING_FOR_CHUNK:
            {
                if (buffer.size() >= entitySize) {
                    chunksBuffer.append(buffer.left(entitySize));
                    buffer.remove(0, entitySize);
                    chunkedEntity = WAITING_FOR_CHUNK_SIZE;
                    break;
                } else {
                    return false;
                }
            }
        case WAITING_FOR_TRAILLING_HEADERS:
            {
                for (int i = buffer.indexOf("\r\n") ; i != -1 ; i = buffer.indexOf("\r\n")) {
                    // don't starts with \r\n
                    if (i != 0) {
                        QByteArray header = buffer.left(i);
                        buffer.remove(0, i + 2);

                        i = header.indexOf(':');
                        if (i > 0) {
                            if (i + 1 < header.size())
                                request.setHeader(header.left(i).trimmed(), header.mid(i + 1).trimmed());
                            else
                                request.setHeader(header.left(i).trimmed(), QByteArray());
                        } else {
                            throw HttpResponse(request.httpVersion() < lastSupportedHttpVersion ?
                                               request.httpVersion() : lastSupportedHttpVersion,
                                               HttpResponse::BAD_REQUEST, true);
                        }
                    } else {
                        buffer = chunksBuffer;
                        chunksBuffer.clear();

                        return true;
                    }
                }
            }

        // not a chunked entity
        case false:
            if (buffer.size() >= entitySize) {
                buffer.chop(buffer.size() - entitySize);
                return true;
            } else {
                return false;
            }
        }
    }
}

void WebserverSession::onRequest(const HttpRequest &request)
{

    timer->start(SESSION_TIMEOUT);
    if (request.httpVersion() == lastSupportedHttpVersion &&
        request.header("HOST").isNull()) {
        writeResponse(HttpResponse(lastSupportedHttpVersion, HttpResponse::BAD_REQUEST, true));
        timer->stop();
        return;
    }
    if (WebService *service = server->service(request.uri())) {
        lastRequest = request;
        service->postRequest(request, this);
    } else if (request.method() == "NOTIFY") {
        if (WebService *service = server->service("/")) {
            qDebug() << request.entityBody();
            if(request.entityBody()) {
                service->postRequest(request, this);
            }
        } else {
            qWarning() << "Service is NULL!";
        }

    } else {
        writeResponse(HttpResponse(request.httpVersion(), HttpResponse::NOT_FOUND, true));
        timer->stop();
    }
}

void WebserverSession::prepareResponse(HttpResponse &r)
{
    if (lastRequest.httpVersion().minor() == 0 || lastRequest.header("CONNECTION") == "close")
        r.setCloseConnection(true);

    QPair<qlonglong, qlonglong> range = r.range();

    if (useChunkedResponse) {
        r.setHeader("TRANSFER-ENCODING", "chunked");
        r.setHeader("CONTENT-LENGTH", QByteArray());
    } else {
        if (r.useRange()) {
            r.setHeader("CONTENT-LENGTH", QByteArray::number(1 + range.second
                                                             - range.first));
        } else {
            r.setHeader("CONTENT-LENGTH", QByteArray::number(r.entitySize()));
        }
    }

    if (r.useRange()) {
        r.setHeader("CONTENT-RANGE",
                    QByteArray("bytes ")
                    + QByteArray::number(range.first) + '-'
                    + QByteArray::number(range.second) + '/'
                    + QByteArray::number(r.entitySize()));
    }
}

void WebserverSession::writeEntityBody(const HttpResponse &r, QTcpSocket *s)
{
    QIODevice *body = r.entityBody();
    QPair<qlonglong, qlonglong> range = r.range();

    if (!r.useRange()) {
        range.first = 0;
        range.second = body->size() - 1;
    }
    body->seek(range.first);
    qlonglong remainingBytes = 1 + range.second - range.first;

    if (useChunkedResponse) {
        QByteArray buffer;

        while (remainingBytes) {
            buffer = body->read((MAPPED_MEMORY_SIZE > remainingBytes)
                                ? remainingBytes : MAPPED_MEMORY_SIZE);
            s->write(QByteArray::number(buffer.size(), 16).toUpper());
            s->write("\r\n");
            s->write(buffer);
            remainingBytes -= buffer.size();
        }

        s->write("0\r\n\r\n");
    } else {
        while (remainingBytes) {
            QByteArray buffer = body->read((MAPPED_MEMORY_SIZE > remainingBytes)
                                           ? remainingBytes : MAPPED_MEMORY_SIZE);
            s->write(buffer);
            remainingBytes -= buffer.size();
        }
    }
}

void WebserverSession::sessionStarted()
{
    timer->start(SESSION_TIMEOUT);
}

bool WebserverSession::keepAlive()
{
    chunksBuffer.clear();
    return true;
}

void WebserverSession::onTimeout()
{
    writeResponse(HttpResponse(HttpVersion(1, 1), HttpResponse::REQUEST_TIMEOUT, true));
}

}  // namespace webserver
}  // namespace shared
}  // namespace brisa
