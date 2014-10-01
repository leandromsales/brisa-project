/*
 * ApplicationUI.cpp
 *
 *  Created on: 09/09/2013
 *      Author: daniel
 */

#include "ApplicationUI.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app) {
	ApplicationInfo::registerQmlTypes();
	// create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///qml/main.qml").parent(
			this);

	// create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();

	// Create a SceneCover and set the application cover
    ActiveFrameQML *activeFrame = new ActiveFrameQML();
    Application::instance()->setCover(activeFrame);

	qRegisterMetaType<ArrayDataModel *>("ArrayDataModel *");
	qRegisterMetaType<ArrayDataModel *>("GroupDataModel *");

	// Create ViewController controller. It must be a pointer.
	QmlDocument::defaultDeclarativeEngine()->rootContext()->setContextProperty("_viewController", new ViewController(this));
//	qml->setContextProperty("_viewController", new ViewController(this));
	qml->documentContext()->setContextProperty("_actionController",
			new ActionController(this));
	qml->documentContext()->setContextProperty("_executableAction",
			ExecutableAction::getInstance(this));

	// set created root object as a scene
	app->setScene(root);
}
