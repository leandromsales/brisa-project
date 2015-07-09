#include <QObject>
#include <QString>

class DataObject : public QObject {
    Q_OBJECT

//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

public:
    DataObject();
    DataObject(QString name, QString color);
    ~DataObject();

private:
    QString name;
    QString color;
};
