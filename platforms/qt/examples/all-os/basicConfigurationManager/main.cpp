#include <QtCore>
#include <QCoreApplication>
#include <BrisaCore/BrisaConfigurationManager>
#include <BrisaUtils/BrisaLog>

using namespace Brisa;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    brisaLogInitialize();

    QString configPath("./");

    //generate my own config
    QHash<QString,QString> state;
    state["brisaPython.owner"] = "owner1";
    state["brisaPython.version"] = "0.10.0";
    state["brisaPython.encoding"] = "utf-8'";
    state["brisaC++.owner"] = "owner2";
    state["brisaC++.version"] = "0.1.0";
    state["brisaC++.encoding"] = "utf-8";

    BrisaConfigurationManager *myConfig = new BrisaConfigurationManager(configPath, state);

    //save the config
    myConfig->save();
    qDebug() << "value: " << myConfig->getParameter("brisaC++","owner");
    myConfig->setParameter("brisaC++", "owner", "newOwner");
    myConfig->save();

    //update the config
    myConfig->update();
    qDebug() << "value: " << myConfig->getParameter("brisaC++","listenif");
	foreach (QString confs, myConfig->getSectionNames()) {
		qDebug() << confs;
	}

    return app.exec();
}

