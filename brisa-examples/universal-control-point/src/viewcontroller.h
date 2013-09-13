#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <qdeclarativecontext.h>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/QListDataModel>
#include <bb/cascades/GroupDataModel>


#include "mediaitem.h"
#include "mediaitemlocal.h"
#include "mediacontentitem.h"
#include "mediaplayermodel.h"
#include "upnp/device/av_device/mediarendererdevice.h"
#include "upnp/controlpoint/av_controlpoint/mediacontrolpoint.h"

using namespace bb::cascades;

class ViewController : public QObject
{
    Q_OBJECT
public:
    explicit ViewController(QmlDocument * context, QObject *parent = 0);
    QmlDocument * uiCtx;
    ~ViewController();
    void executeBrowse(Service *service, const QString &objectId);
    void executeSetAVTransportURI(Service *service);
    void executePlay(Service *service);
    void executeStop(Service *service);
    void executePause(Service *service);
    void executeSetVolume(Service *service, const QString &desiredVolume);
    void executeGetTransportInfo(Service *service);
    void init();
    void goesToQML();
    void playLocal();
    GroupDataModel * getMediaServerModel();
    ListModel * getMediaServerContentModel();
    GroupDataModel * getMediaRendererModel();
    ListModel * getNavigationContentModel();
signals:
    void emptyMediaServerContentList();
    void changeStatusToPlay(bool toPlay);

public slots:
void changeVolume(int value);
    void onPlayPausePressed(bool isPlaying);
    void onStopButtonPressed();
    void onBackButtonPressed();
    void onChangeSelectionForServerList(QString uuid);
    void onChangeSelectionForRendererList(QString uuid);
    void onChangeSelectionForServerContentList(QString objectId);
    void onSelectionChangedFolderNavigator(QString objectId);
    void positionChanged(qint64 position);
    void newMediaServerDevice(const MediaServerControlPointDevice *);
    void newMediaRendererDevice(const MediaRendererControlPointDevice *);
    void leaveMediaServerDevice(const QString &);
    void leaveMediaRendererDevice(const QString &);
    void handleExecutionActionReply(QString actionName,
                                    QHash<QString, ActionArgument *> response,
                                    Service *service);

private:
    MediaControlPoint * mediaControlPoint;
    MediaRendererDevice * mediaRendererDevice;
    //ListModel * mediaServerModel;
    bb::cascades::GroupDataModel * mediaServerModel;
    //ListModel * mediaRendererModel;
    bb::cascades::GroupDataModel * mediaRendererModel;
    ListModel * mediaServerContentModel;
    ListModel * navigationContentModel;

    Device * currentMediaServer;
    Device * currentMediaRenderer;
    QString currentURI;
    MediaPlayerModel * mediaPlayerModel;
    bool m_playlocal;
};

#endif // VIEWCONTROLLER_H
