// Default empty project template
#include <bb/cascades/Application>


#include <QLocale>
#include <QTranslator>
#include "applicationui.hpp"
//Marden
#include "upnp/controlpoint/av_controlpoint/controlpoint_media_globals.h"
#include "upnp/shared/webserver/webserver.h"

// include JS Debugger / CS Profiler enabler
// this feature is enabled by default in the debug build only
#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;

void myMessageOutput(QtMsgType type, const char* msg) {
	fprintf(stdout, "%s\n", msg);
	fflush (stdout);
	Q_UNUSED(type);
}

Q_DECL_EXPORT int main(int argc, char **argv) {
	// this is where the server is started etc
	Application app(argc, argv);
	#ifndef QT_NO_DEBUG
	qInstallMsgHandler(myMessageOutput);
	#endif

	// localization support
	QTranslator translator;
	QString locale_string = QLocale().name();
	QString filename = QString("upnplayer_%1").arg(locale_string);
	if (translator.load(filename, "app/native/qm")) {
		app.installTranslator(&translator);
	}

	new ApplicationUI(&app);

	// we complete the transaction started in the app constructor and start the client event loop here
	return Application::exec();
	// when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
