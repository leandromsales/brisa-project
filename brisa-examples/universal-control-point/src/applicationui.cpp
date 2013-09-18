// Default empty project template
#include "applicationui.hpp"
#include "viewcontroller.h"
#include "servicecontroller.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ListView>
#include <bb/cascades/ArrayDataModel>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
{
	//m_model = new GroupDataModel();
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///qml/main.qml").parent(this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    qRegisterMetaType<ArrayDataModel *>("ArrayDataModel *");

    // Create ViewController controller. It must be a pointer.
    ViewController *vc = new ViewController(qml);
    vc->goesToQML();
    ListView *cp_listview = root->findChild<ListView*>("control_point_listview");
    cp_listview->setDataModel(vc->getControlPointModel());
    // set created root object as a scene
    app->setScene(root);
}

