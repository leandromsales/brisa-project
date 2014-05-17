#include "brisastatevariable.h"
#include "brisaabstractservice.h"

namespace brisa {
namespace upnp {

StateVariable::StateVariable(QString sendEvents,
                                       QString name,
                                       QString datatype,
                                       QString defaultValue,
                                       QString maximum,
                                       QString minimum,
                                       QString step,
                                       QString multicast,
                                       QObject *parent) :
    QObject(parent)
{
    this->nextMulticastSeq = 0;
    this->_sendEvents = (sendEvents == "yes");
    this->_multicast = (multicast == "yes");
    this->_name = name;
    this->_dataType = datatype;
    this->_defaultValue = defaultValue;
    this->_maximum = maximum;
    this->_minimum = minimum;
    this->_step = step;
    this->_value = this->defaultValue;
}

StateVariable::StateVariable(const StateVariable &variable) :
    QObject(variable.parent())
{
    this->_sendEvents = variable.sendEventsChange();
    this->_name = variable.getAttribute(Name);
    this->_dataType = variable.getAttribute(DataType);
    this->_defaultValue = variable.getAttribute(DefaultValue);
    this->_maximum = variable.getAttribute(Maximum);
    this->_minimum = variable.getAttribute(Minimum);
    this->_step = variable.getAttribute(Step);
    this->_value = variable.getValue();
}

StateVariable &StateVariable::operator=(const StateVariable &variable) {
    if (this != &variable) {
        this->nextMulticastSeq = 0;
        this->setParent(variable.parent());
        this->_sendEvents = variable.sendEventsChange();
        this->_name = variable.getAttribute(Name);
        this->_dataType = variable.getAttribute(DataType);
        this->_defaultValue = variable.getAttribute(DefaultValue);
        this->_maximum = variable.getAttribute(Maximum);
        this->_minimum = variable.getAttribute(Minimum);
        this->_step = variable.getAttribute(Step);
        this->_value = variable.getValue();
        this->_multicast = variable.multicastEvents();
    }
    return *this;
}

void StateVariable::setAttribute(BrisaStateVariableAttribute attr, QVariant newValue) {
    switch (attr) {
    case SendEvents:
    case sendEvents:
        this->_sendEvents = (newValue.toString() == "yes");
        break;
    case Name:
    case name:
        this->_name = newValue.toString();
        break;
    case DataType:
    case dataType:
        this->_dataType = newValue.toString();
        break;
    case DefaultValue:
    case defaultValue:
        this->_defaultValue = newValue.toString();
        break;
    case AllowedValue:
    case allowedValue:
        this->allowedValueList.append(newValue.toString());
        break;
    case Minimum:
    case minimum:
        this->_minimum = newValue.toString();
        break;
    case Maximum:
    case maximum:
        this->_maximum = newValue.toString();
        break;
    case Step:
    case step:
        this->_step = newValue.toString();
        break;
    case Value:
    case value:
        if (this->validateNewValue(newValue) && this->_value != newValue) {
            this->_value = newValue;
            if (this->_sendEvents) {
                emit this->changed(this);
            }
        }
        break;
    case Multicast:
    case multicast:
        this->_multicast = (newValue.toString() == "yes");
        break;
    default:
        break;
    }
}

QString StateVariable::getAttribute(BrisaStateVariableAttribute attr, int ind) const {
    switch (attr) {
    case Name:
    case name:
        return this->_name;
        break;
    case DataType:
    case dataType:
        return this->_dataType;
        break;
    case DefaultValue:
    case defaultValue:
        return this->_defaultValue;
        break;
    case AllowedValue:
    case allowedValue:
        return this->allowedValueList[ind];
        break;
    case Minimum:
    case minimum:
        return QString(this->_minimum);
        break;
    case Maximum:
    case maximum:
        return QString(this->_maximum);
        break;
    case Step:
    case step:
        return QString(this->_step);
        break;
    case Value:
    case value:
        return QString(this->_value.toString());
        break;
    case Multicast:
    case multicast:
        return QString(this->_multicast);
        break;
    default:
        return "";
        break;
    }
}

QList<QString> StateVariable::getAllowedValueList() {
    return this->allowedValueList;
}

void StateVariable::addAllowedValue(QString allowedVal) {
    allowedValueList.append(allowedVal);
}

void StateVariable::clear() {
    this->allowedValueList.clear();
    this->_name.clear();
    this->_dataType.clear();
    this->_defaultValue.clear();
    this->_maximum.clear();
    this->_minimum.clear();
    this->_step.clear();
    this->_value.clear();
}

QVariant StateVariable::getValue() const {
    return this->_value;
}

bool StateVariable::sendEventsChange() const {
    return this->_sendEvents;
}

bool StateVariable::multicastEvents() const {
    return this->_multicast;
}

bool StateVariable::validateNewValue(const QVariant &value) {
    QVariant::Type type = this->getDataType();

    if (!value.canConvert(type)) {
        qDebug() << "Type mismatch while validating new value for state variable " << this->_name;
        return false;
    }

    if (type == QVariant::String) {
        if (this->allowedValueList.isEmpty())
            return true;
        if (this->allowedValueList.contains(value.toString())) {
            return true;
        } else {
            qDebug() << "State variable " << this->_name << " does not allows value " << value.toString();
            return false;
        }

    } else if (type == QVariant::Bool) {
        if (value.toString() == "0" || value.toString() == "1"
                || value.toString() == "true" || value.toString() == "false"
                || value.toString() == "yes" || value.toString() == "no") {
            return true;
        } else {
            qDebug() << "Value not allowed for boolean type. Use '0', '1', 'true', 'false', 'yes', or 'no'.";
            return false;
        }

    } else if (type == QVariant::UInt) {
        if (this->_maximum == "" || this->_minimum == "") {
            return true;
        } else if (value.toUInt() <= this->_maximum.toUInt() && value.toUInt() >= this->_minimum.toUInt()) {
            return true;
        } else {
            qDebug() << "Value out of range for UInt.";
            return false;
        }

    } else if (type == QVariant::Int) {
        if (this->_maximum == "" || this->_minimum == "") {
            return true;
        } else if (value.toInt() <= this->_maximum.toInt() && value.toInt() >= this->_minimum.toInt()) {
            return true;
        } else {
            qDebug() << "Value out of range for Int.";
            return false;
        }

    } else if (type == QVariant::Double) {
        if (this->_maximum == "" || this->_minimum == "") {
            return true;
        } else if (value.toDouble() <= this->_maximum.toDouble() && value.toDouble() >= this->_minimum.toDouble()) {
            return true;
        } else {
            qDebug() << "Value out of range for Double.";
            return false;
        }

    } else {
        return true;
    }
}

QVariant::Type StateVariable::getDataType() const {
    if (this->_dataType == "ui1") {
        return QVariant::UInt;
    } else if (this->_dataType == "ui2") {
        return QVariant::UInt;
    } else if (this->_dataType == "ui4") {
        return QVariant::UInt;
    } else if (this->_dataType == "i1") {
        return QVariant::Int;
    } else if (this->_dataType == "i2") {
        return QVariant::Int;
    } else if (this->_dataType == "i4") {
        return QVariant::Int;
    } else if (this->_dataType == "int") {
        return QVariant::Int;
    } else if (this->_dataType == "r4") {
        return QVariant::Double;
    } else if (this->_dataType == "r8") {
        return QVariant::Double;
    } else if (this->_dataType == "number") {
        return QVariant::Double;
    } else if (this->_dataType == "fixed") {
        return QVariant::Double;
    } else if (this->_dataType == "float") {
        return QVariant::Double;
    } else if (this->_dataType == "char") {
        return QVariant::Char;
    } else if (this->_dataType == "string") {
        return QVariant::String;
    } else if (this->_dataType == "date") {
        return QVariant::Date;
    } else if (this->_dataType == "dateTime") {
        return QVariant::DateTime;
    } else if (this->_dataType == "dateTime.tz") {
        return QVariant::DateTime;
    } else if (this->_dataType == "time") {
        return QVariant::Time;
    } else if (this->_dataType == "time.tz") {
        return QVariant::Time;
    } else if (this->_dataType == "boolean") {
        return QVariant::Bool;
    } else if (this->_dataType == "bin.base64") {
        return QVariant::String;
    } else if (this->_dataType == "bin.hex") {
        return QVariant::String;
    } else if (this->_dataType == "uri") {
        return QVariant::String;
    } else if (this->_dataType == "uuid") {
        return QVariant::String;
    } else {
        return QVariant::Invalid;
    }
}

void StateVariable::setMulticastSeq(quint32 *seq)
{
    this->nextMulticastSeq = seq;
}

quint32 StateVariable::getNextMulticastSeq()
{
    if (*(this->nextMulticastSeq) == 0) {
        (*(this->nextMulticastSeq))++;
        return 0;
    }

    if (*(this->nextMulticastSeq) == 0xFFFFFFFF) /*4294967295L*/
        *(this->nextMulticastSeq) = 0;

    return (*(this->nextMulticastSeq))++;
}

void StateVariable::setMulticastUsn(QString usn)
{
    this->multicastUsn = usn;
}

QString StateVariable::getMulticastUsn()
{
    return this->multicastUsn;
}

void StateVariable::setMulticastUdn(QString udn)
{
    this->multicastUdn = udn;
}

QString StateVariable::getMulticastUdn()
{
    return this->multicastUdn;
}

void StateVariable::setMulticastSvcid(QString svcid)
{
    this->multicastSvcid = svcid;
}

QString StateVariable::getMulticastSvcid()
{
    return this->multicastSvcid;
}

}
}
