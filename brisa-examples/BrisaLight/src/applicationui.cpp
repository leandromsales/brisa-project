#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include <bb/cascades/Color>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Page>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{

	// prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);
    if(!QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()))) {
        // This is an abnormal situation! Something went wrong!
        // Add own code to recover here
        qWarning() << "Recovering from a failed connect()";
    }
    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
//    qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Create root object for the UI
//    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
//    app->setScene(root);


	// Create the root container
	contentContainer = new Container();
	contentContainer->setLayout(StackLayout::create());
	contentContainer->setTopPadding(20.0f);

	contentContainer->setBackground(Color::fromARGB(0xff262626));

	Page *page = new Page();
	page->setContent(contentContainer);
	app->setScene(page);

	binaryLight = new BinaryLight(this);
	binaryLight->start();
}

void ApplicationUI::statechanged(StateVariable *var) {
    if (var->getValue().toBool()) {
    	contentContainer->setBackground(Color::White);
    } else {
    	contentContainer->setBackground(Color::Black);
    }
    qDebug() << "State changed: " << var->getValue().toBool();
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("WANChat_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
