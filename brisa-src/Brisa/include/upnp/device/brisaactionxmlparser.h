#ifndef BRISAACTIONXMLPARSER_H_
#define BRISAACTIONXMLPARSER_H_

#include <QtCore>
#include <QDomDocument>

namespace brisa {
/*!
 * \class Brisa::BrisaActionXmlParser brisaactionxmlparser.h BrisaUpnp/BrisaActionXmlParser
 * \brief XML parser for SOAP requests.
 *
 * BrisaActionXmlParser parses information coming from the webserver. If a action is detected,
 * public class members args, method, serviceType will be filled with the parsed data.
 *
 * BrisaActionXmlParser uses DOM.
 */
class BrisaActionXmlParser {
public:

    /*!
     * Constructor.
     */
    BrisaActionXmlParser();

    /*!
     * Destructor.
     */
    virtual ~BrisaActionXmlParser();

    /*!
     * Call this method to parse the SOAP request set by the setXmlContent method.
     */
    bool parseSOAP();

    /*!
     * \internal
     * Parses each DOM element.
     * \param element element to be parsed
     */
    void parseElement(QDomElement &element);

    /*!
     * Sets  the content to be parsed.
     * \param content the content to be parsed
     */
    void setXmlContent(const QByteArray &content);

    QMap<QString, QString> args;
    QString method;
    QString serviceType;

private:
    QDomDocument *reader;
    QDomNodeList domArgList;
};

}

#endif /* XMLPARSER_H_ */
