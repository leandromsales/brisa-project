#include <QObject>
#include <QString>
#include <QUrl>

#include "Controller/essencials/qqmlhelpers.h"

class DataObject : public QObject {
    Q_OBJECT

    QML_WRITABLE_PROPERTY(QString, name)
    QML_WRITABLE_PROPERTY(QString, info)
    QML_WRITABLE_PROPERTY(QString, udn)
    QML_WRITABLE_PROPERTY(QUrl, iconURL)
    QML_WRITABLE_PROPERTY(QUrl, appURL)

public:
    DataObject();
    DataObject(QString udn, QString name, QString info, QUrl iconURL, QUrl appURL);
    ~DataObject();
};
