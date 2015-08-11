#include "rendercontrol.h"

using namespace Brisa;
using namespace Phonon;

RenderControl::RenderControl(AudioOutput *playerAudio) :
        BrisaService("urn:schemas-upnp-org:service:RenderingControl:1",
                     "RenderingControl",
                     "/RenderingControl/render-control-scpd.xml",
                     "/RenderingControl/control",
                     "/RenderingControl/eventSub")
{
    this->audio = playerAudio;
    setDescriptionFile("render-control-scpd.xml");
}

RenderControl::~RenderControl()
{
    delete this->audio;
}

BrisaOutArgument* RenderControl::listpresets(BrisaInArgument *const inArguments,
                                             BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    //InstanceID is an input argument.
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("CurrentPresetNameList", "");
    return outArgs;
}

BrisaOutArgument* RenderControl::selectpreset(BrisaInArgument *const inArguments,
                                             BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    //InstanceID is an input argument.
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    return outArgs;
}

BrisaOutArgument* RenderControl::getvolume(BrisaInArgument *const inArguments,
                                           BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //InstanceID and Channel are input arguments.
    BrisaOutArgument *outArgs = new BrisaOutArgument();

    outArgs->insert("CurrentVolume", getVariable("Volume")->getAttribute(
            BrisaStateVariable::Value));

    return outArgs;
}

BrisaOutArgument* RenderControl::setvolume(BrisaInArgument *const inArguments,
                                           BrisaAction *const action)
{
    Q_UNUSED(action)
    //InstanceID, Channel and DesiredVolume are input arguments.
    audio->setVolume(inArguments->value("DesiredVolume").toUInt());
    getVariable("Volume")->setAttribute(BrisaStateVariable::Value,
                                             inArguments->value("DesiredVolume"));
    return new BrisaOutArgument();
}
