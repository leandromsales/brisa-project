#include <QObject>
#include <QString>

class DataObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString info READ getInfo WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QString iconURL READ getIconURL WRITE setIconURL NOTIFY iconURLChanged)
    Q_PROPERTY(QString appURL READ getAppURL WRITE setAppURL NOTIFY appURLChanged)

public:
    DataObject();
    DataObject(QString name, QString info, QUrl iconURL, QUrl appURL);
    ~DataObject();
    Q_INVOKABLE QString getName();
    Q_INVOKABLE QString getInfo();
    Q_INVOKABLE QString getIconURL();
    Q_INVOKABLE QString getAppURL();
    Q_INVOKABLE void setName(QString newName);
    Q_INVOKABLE void setInfo(QString newInfo);
    Q_INVOKABLE void setIconURL(QString newIcon);
    Q_INVOKABLE void setAppURL(QString newApp);

signals:
    void nameChanged();
    void infoChanged();
    void iconURLChanged();
    void appURLChanged();

public:
    QString name;
    QString info;
    QUrl iconURL;
    QUrl appURL;
};
