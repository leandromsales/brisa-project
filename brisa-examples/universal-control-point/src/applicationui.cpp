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
    QmlDocument *qml = QmlDocument::create("qrc:///gui/bb10/qml/main.qml").parent(this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Create ViewController controller. It must be a pointer.
    ViewController *vc = new ViewController(qml);
    vc->goesToQML();
    ListView *ms_listview = root->findChild<ListView*>("media_server_listview");
    ms_listview->setDataModel(vc->getMediaServerModel());
    ListView *mr_listview = root->findChild<ListView*>("media_renderer_listview");
    mr_listview->setDataModel(vc->getMediaRendererModel());
    // set created root object as a scene
    app->setScene(root);
}

