#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Container>

#include "BinaryLight.h"
#include <upnp/brisastatevariable.h>

namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
    }
}

class QTranslator;
class BinaryLight;

/*!
 * @brief Application object
 *
 *
 */

using namespace bb::cascades;
using namespace Brisa;

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {
    	qDebug() << "Destrutor";
//    	delete binaryLight;
    }
private slots:
    void onSystemLanguageChanged();

public slots:
	void statechanged(StateVariable *);

private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    QmlDocument *qml;
    Container *contentContainer;

    BinaryLight *binaryLight;

};

#endif /* ApplicationUI_HPP_ */
