#ifndef HTTPSESSIONMANAGER_H
#define HTTPSESSIONMANAGER_H

#include <QThread>
#include <QMutex>
#include "httpserver.h"

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

class HttpSessionManager : public QThread
{
Q_OBJECT
public:
    explicit HttpSessionManager(HttpServer *parent);
    /*!
     * Emit a signal onNewConnection with \param socketDescriptor.
     */
    void addSession(qintptr socketDescriptor);
    /*!
     * Add a new session to pool.
     */
    void releaseSession(HttpSession *);

signals:
    void newConnection(qintptr);

private slots:
    /*!
     * Set a HTTP Session of pool with socket descriptor \param socketDescriptor.
     */
    void onNewConnection(qintptr socketDescriptor);

private:
    HttpServer *server;
    QList<HttpSession *> pool;
    QMutex mutex;
};

}  // namespace http
}  // namespace webserver
}  // namespace shared
}  // namespace Brisa

#endif // HTTPSESSIONMANAGER_H
