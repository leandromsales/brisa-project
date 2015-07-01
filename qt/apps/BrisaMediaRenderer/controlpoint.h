#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include <QObject>

#include <BrisaControlPoint>

using Brisa::BrisaControlPoint;
using Brisa::BrisaControlPointDevice;

class ControlPoint : public QObject
{
Q_OBJECT
public:
    explicit ControlPoint(QObject *parent = 0);
    ~ControlPoint();
    void start();
    void discover();
    void parseBrowse(QString result);
    void parseSearch(QString result);

signals:
    void mediaServerFound(BrisaControlPointDevice*);
    void mediaServerGone(QString udn);
    void mediaServersChanged(QMap<QString, BrisaControlPointDevice*>);

public slots:
    void newDevice(BrisaControlPointDevice *device);
    void deleteDevice(QString udn);
    void browse(QString udn);
    void search(QString udn);

private:
    BrisaControlPoint *cp;
    QMap<QString, BrisaControlPointDevice*> mediaServers;
};

#endif // CONTROLPOINT_H
