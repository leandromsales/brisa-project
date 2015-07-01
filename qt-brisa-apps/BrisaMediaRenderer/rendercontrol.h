#ifndef RENDERCONTROL_H
#define RENDERCONTROL_H

#include <BrisaService>
#include <phonon/AudioOutput>

using Brisa::BrisaService;
using Brisa::BrisaAction;

using Phonon::AudioOutput;
/*!
 * \internal
 *
 * \class Brisa::RenderControl
 *
 * \brief Control the rendering flow.
 */
class RenderControl: public BrisaService {
Q_OBJECT

public:
    /*!
     * \internal
     *
     * Create a new instance object of this class.
     */
    RenderControl(AudioOutput *playerAudio);

    virtual ~RenderControl();

public slots:
    /*!
     * \internal
     *
     * List the current presets.
     */
    BrisaOutArgument* listpresets(BrisaInArgument* const inArguments, BrisaAction* const action);
    /*!
     * \internal
     *
     * Select o preset to apply.
     */
    BrisaOutArgument* selectpreset(BrisaInArgument* const inArguments,
            BrisaAction* const action);
    /*!
     * \internal
     *
     * Get the current volume level.
     */
    BrisaOutArgument* getvolume(BrisaInArgument* const inArguments,
            BrisaAction* const action);
    /*!
     * \internal
     *
     * Set the current volume level.
     */
    BrisaOutArgument* setvolume(BrisaInArgument* const inArguments,
            BrisaAction* const action);

private:
    AudioOutput *audio;
};

#endif // RENDERCONTROL_H
