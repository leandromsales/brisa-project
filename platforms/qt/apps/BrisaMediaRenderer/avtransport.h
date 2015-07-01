#ifndef AVTRANSPORT_H
#define AVTRANSPORT_H

#include <BrisaService>
#include <phonon/MediaObject>

using Brisa::BrisaService;
using Brisa::BrisaAction;

using Phonon::MediaObject;

class AVTransport : public BrisaService
{
    Q_OBJECT
public:
    AVTransport(MediaObject *player);
    virtual ~AVTransport();

public slots:
    BrisaOutArgument* setavtransporturi(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* getmediainfo(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* gettransportinfo(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* getpositioninfo(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* getdevicecapabilities(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* gettransportsettings(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* stop(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* play(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* pause(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* seek(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* next(BrisaInArgument * const inArguments, BrisaAction * const action);
    BrisaOutArgument* previous(BrisaInArgument * const inArguments, BrisaAction * const action);

    void downloadFinished(QNetworkReply *reply);

private:
    MediaObject *player;
    QByteArray currentURI;
    QNetworkAccessManager *manager;

};


#endif // AVTRANSPORT_H
