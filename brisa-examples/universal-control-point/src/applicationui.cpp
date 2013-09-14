// Default empty project template
#include "applicationui.hpp"
#include "viewcontroller.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ListView>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
{
	//m_model = new GroupDataModel();
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("qrc:///assets/qml/main.qml").parent(this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

//    qml->setContextProperty("nomeVariavavel", this);

    QmlDocument *qmlServices = QmlDocument::create("qrc:///assets/qml/Services.qml").parent(this);

    // create root object for the UI
    AbstractPane *rootServices = qmlServices->createRootObject<AbstractPane>();

    // Create ViewController controller. It must be a pointer.
    ViewController *vc = new ViewController(qml);
    vc->goesToQML();
    ListView *cp_listview = root->findChild<ListView*>("control_point_listview");
    cp_listview->setDataModel(vc->getControlPointModel());
    ListView *sv_listview = rootServices->findChild<ListView*>("services_listview");
    sv_listview->setDataModel(vc->getControlPointModel());
    // set created root object as a scene
    app->setScene(root);
}

