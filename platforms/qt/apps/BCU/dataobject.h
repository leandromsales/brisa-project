#include <QObject>
#include <QString>

class DataObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString info READ getInfo WRITE setInfo NOTIFY infoChanged)

public:
    DataObject();
    DataObject(QString name, QString info);
    ~DataObject();
    Q_INVOKABLE QString getName();
    Q_INVOKABLE QString getInfo();
    Q_INVOKABLE void setName(QString newName);
    Q_INVOKABLE void setInfo(QString newInfo);

signals:
    void nameChanged();
    void infoChanged();

public:
    QString name;
    QString info;
};
