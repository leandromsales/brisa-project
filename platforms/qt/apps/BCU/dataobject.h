#include <QObject>
#include <QString>
#include <QUrl>

class DataObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString info READ getInfo WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QUrl iconURL READ getIconURL WRITE setIconURL NOTIFY iconURLChanged)
    Q_PROPERTY(QUrl appURL READ getAppURL WRITE setAppURL NOTIFY appURLChanged)
    Q_PROPERTY(QString section READ getSection WRITE setSection NOTIFY sectionChanged)

public:
    DataObject();
    DataObject(QString udn, QString name, QString info, QUrl iconURL, QUrl appURL, QString section);
    ~DataObject();
    QString getUdn();
    Q_INVOKABLE QString getName();
    Q_INVOKABLE QString getInfo();
    Q_INVOKABLE QUrl getIconURL();
    Q_INVOKABLE QUrl getAppURL();
    Q_INVOKABLE QString getSection();
    void setUdn (QString newUdn);
    Q_INVOKABLE void setName(QString newName);
    Q_INVOKABLE void setInfo(QString newInfo);
    Q_INVOKABLE void setIconURL(QUrl newIcon);
    Q_INVOKABLE void setAppURL(QUrl newApp);
    Q_INVOKABLE void setSection(QString newSection);

signals:
    void nameChanged();
    void infoChanged();
    void iconURLChanged();
    void appURLChanged();
    void sectionChanged();

public:
    QString name;
    QString info;
    QUrl iconURL;
    QUrl appURL;
    QString udn;
    QString section;
};
