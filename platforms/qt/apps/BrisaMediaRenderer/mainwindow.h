#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore>

#include <phonon/MediaObject>
#include <phonon/BackendCapabilities>
#include <phonon/AudioOutput>
#include <phonon/Path>
#include <phonon/VideoWidget>

#include <BrisaDevice>

#include "controlpoint.h"
#include "avtransport.h"
#include "rendercontrol.h"
#include "connectionmanager.h"

using Brisa::BrisaDevice;

using Phonon::VideoWidget;
using Phonon::MediaObject;
using Phonon::AudioOutput;

namespace Ui
{
    class MainWindow;
}
/*!
 * \internal
 *
 * \class Brisa::MainWindow
 *
 * \brief Represents the main window of Media Renderer.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \internal
     *
     * Create a new window.
     */
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    AVTransport *avTransport;
    RenderControl *renderControl;
    ConnectionManager *connectionManager;

    VideoWidget *videoWidget;
    MediaObject *source;
    AudioOutput *audio;

    BrisaDevice *device;
    ControlPoint *cp;
};

#endif // MAINWINDOW_H
