#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <QObject>
#include <QString>

namespace brisa {
namespace upnp {

/*!
 * \internal
 *
 * \class Brisa::BrisaArgument brisaargument.h BrisaUpnp/BrisaArgument
 *
 * \brief Represents the action's input and output arguments.
 */
class Argument: public QObject {
    Q_OBJECT
public:

    /*!
     * \internal
     *
     * Constructs an argument with given \a name, \a direction and
     * \a relatedStateVariable.
     */
    Argument(const QString &name = "", const QString &direction = "",
            const QString &relatedStateVariable = "", QObject *parent = 0);

    /*!
     * \internal
     *
     * Constructs an argument with an argument reference.
     */
    Argument(const Argument &brisaArgument);

    typedef enum {
        ArgumentName, Direction, RelatedStateVariable
    } xmlArgument;

    /*!
     * \internal
     *
     * Sets the argument's attribute \a key to the given \a value.
     */
    void setAttribute(xmlArgument key, const QString &value);

    /*!
     * \internal
     *
     * Returns the value of the argument's attribute \a key.
     */
    QString getAttribute(xmlArgument key) const;

    /*!
     * \internal
     *
     * Sets the argument's attributes to their default value.
     */
    void clear();

private:
    QString name;
    QString direction;
    QString relatedStateVariable;
};

}
}

#endif /* ARGUMENT_H_ */
