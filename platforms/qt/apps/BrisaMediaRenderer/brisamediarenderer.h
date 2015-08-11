#ifndef BRISAMEDIARENDERER_H
#define BRISAMEDIARENDERER_H

#include <QObject>
#include <QListView>
#include <QStringListModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <BrisaDevice>
#include "controlpoint.h"

using Brisa::BrisaDevice;
using Brisa::BrisaControlPointDevice;

class BrisaMediaRenderer : public QWidget
{
Q_OBJECT
public:
    explicit BrisaMediaRenderer(QWidget *parent = 0);
    virtual ~BrisaMediaRenderer();
    void configureWidget();

signals:

public slots:
    void mediaServersChanged(QMap<QString, BrisaControlPointDevice*> devices);
    void browse(QModelIndex index);

private:
    BrisaDevice *device;
    ControlPoint *cp;
    QStringListModel *model;
    QListView *mediaServers;
    QPushButton *play;
    QPushButton *stop;
    QPushButton *pause;
    QHBoxLayout *msLayout;
    QVBoxLayout *mrLayout;
    QHBoxLayout *mediaControl;
    QHBoxLayout *mainLayout;
};

#endif // BRISAMEDIARENDERER_H
