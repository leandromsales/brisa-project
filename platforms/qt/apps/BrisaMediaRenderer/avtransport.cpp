#include "avtransport.h"

using namespace Brisa;
using namespace Phonon;

AVTransport::AVTransport(Phonon::MediaObject *player) :
        BrisaService("urn:schemas-upnp-org:service:AVTransport:1",
                     "AVTransport",
                     "/AVTransport/render-transport-scpd.xml",
                     "/AVTransport/control",
                     "/AVTransport/eventSub")
{
    this->player = player;
    setDescriptionFile("render-transport-scpd.xml");
    this->manager = new QNetworkAccessManager();
}

AVTransport::~AVTransport()
{
    delete this->player;
    delete this->manager;
}

BrisaOutArgument* AVTransport::setavtransporturi(
        BrisaInArgument *const inArguments, BrisaAction *const action)
{
    Q_UNUSED(action)

    if (!inArguments->contains("CurrentURI"))
        return new BrisaOutArgument();

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QUrl url(inArguments->value("CurrentURI"));
    manager->get(QNetworkRequest(url));
    getVariable("AVTransportURI")->setAttribute(BrisaStateVariable::Value,
                                                     inArguments->value("CurrentURI"));
    getVariable("TransportState")->setAttribute(BrisaStateVariable::Value, "STOPPED");
    getVariable("NumberOfTracks")->setAttribute(BrisaStateVariable::Value, "1");

    return new BrisaOutArgument();
}

BrisaOutArgument* AVTransport::getmediainfo(BrisaInArgument *const inArguments,
                                            BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("NrTracks",
                   getVariable("NumberOfTracks")->getValue().toString());

    outArgs->insert("MediaDuraction",
                   getVariable("CurrentMediaDuration")->getValue().toString());

    outArgs->insert("CurrentURI",
                   getVariable("AVTransportURI")->getValue().toString());

    outArgs->insert("NextURI",
                   getVariable("NextAVTransportURI")->getValue().toString());

    outArgs->insert("PlayMedium",
                   getVariable("PlaybackStorageMedium")->getValue().toString());

    outArgs->insert("CurrentURIMetaData", "");
    outArgs->insert("NextURIMetaData", "");
    outArgs->insert("RecordMedium", "");
    outArgs->insert("WriteStatus", "");
    return outArgs;
}

BrisaOutArgument* AVTransport::gettransportinfo(BrisaInArgument *const inArguments,
                                                BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    //InstanceID is an input argument.
    BrisaOutArgument *outArgs = new BrisaOutArgument();

    outArgs->insert("NrTracks",
                   getVariable("NumberOfTracks")->getValue().toString());
    outArgs->insert("CurrentTransportState",
                   getVariable("TransportState")->getValue().toString());
    outArgs->insert("CurrentTransportStatus", "");
    outArgs->insert("CurrentSpeed", "");
    return outArgs;
}

BrisaOutArgument* AVTransport::getpositioninfo(
        BrisaInArgument *const inArguments, BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    //InstanceID is an input argument.
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("Track", "");
    outArgs->insert("TrackDuration", "");
    outArgs->insert("TrackMetaData", "");
    outArgs->insert("TrackURI", "");
    outArgs->insert("RelTime", "");
    outArgs->insert("AbsTime", "");
    outArgs->insert("RelCount", "");
    outArgs->insert("AbsCount", "");
    return outArgs;
}

BrisaOutArgument* AVTransport::getdevicecapabilities(
        BrisaInArgument *const inArguments, BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    //InstanceID is an input argument.
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("PlayMedia", "");
    outArgs->insert("RecMedia", "");
    outArgs->insert("RecQualityModes", "");
    return outArgs;
}

BrisaOutArgument* AVTransport::gettransportsettings(
        BrisaInArgument *const inArguments, BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    //InstanceID is an input argument.
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("PlayMode", "");
    outArgs->insert("RecQualityMode", "");
    return outArgs;
}

BrisaOutArgument* AVTransport::stop(BrisaInArgument *const inArguments,
                                    BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    //InstanceID is an input argument.
    //Speed is an input argument.

    this->player->stop();
    getVariable("TransportState")->setAttribute(BrisaStateVariable::Value, "STOPPED");

    return new BrisaOutArgument();
}

BrisaOutArgument* AVTransport::play(BrisaInArgument *const inArguments,
                                    BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //InstanceID is an input argument.
    //Speed is an input argument.

    QString uri = getVariable("AVTransportURI")->getAttribute(BrisaStateVariable::Value);

    qDebug() << this->getVariable("AVTransportURI")->getAttribute(BrisaStateVariable::Value);
//    this->player->setCurrentSource(MediaSource(QUrl(
//            this->getVariable("AVTransportURI")->getAttribute(
//                    BrisaStateVariable::Value))));
//    this->player->play();
    if (getVariable("TransportState")->getAttribute(BrisaStateVariable::Value) == "STOPPED") {
        if (uri.contains("http")) {
            player->setCurrentSource(MediaSource(QUrl(uri)));
        } else {
            player->setCurrentSource(MediaSource(uri));
        }
    }

    player->play();
    getVariable("TransportState")->setAttribute(BrisaStateVariable::Value, "PLAYING");

    return new BrisaOutArgument();
}

BrisaOutArgument* AVTransport::pause(BrisaInArgument *const inArguments,
                                     BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //InstanceID is an input argument.

    player->pause();
    getVariable("TransportState")->setAttribute(BrisaStateVariable::Value,
                                                     "PAUSED_PLAYBACK");
    return new BrisaOutArgument();
}

BrisaOutArgument* AVTransport::seek(BrisaInArgument *const inArguments,
                                    BrisaAction *const action)
{
    Q_UNUSED(action)
    //InstanceID is an input argument.
    //Unit is an input argument.
    //Target is an input argument.

    if (inArguments->value("Unit") == "ABS_TIME")
        player->seek(inArguments->value("Unit").toInt());

    return new BrisaOutArgument();
}

BrisaOutArgument* AVTransport::next(BrisaInArgument *const inArguments,
                                    BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: Everything!
    //InstanceID is an input argument.

    return new BrisaOutArgument();
}

BrisaOutArgument* AVTransport::previous(BrisaInArgument *const inArguments,
                                        BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: Everything!
    //InstanceID is an input argument.

    return new BrisaOutArgument();
}

void AVTransport::downloadFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll().data();
    this->currentURI = data;
    qDebug() << "Downloaded: " << data;
}
