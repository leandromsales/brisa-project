#include "functions.h"

using namespace brisa;
using namespace brisa::upnp;
using namespace brisa::upnp::device;

Functions::Functions() :
    Service(SERVICE_TYPE,
            SERVICE_ID,
            SERVICE_XML_PATH,
            SERVICE_CONTROL,
            SERVICE_EVENT_SUB)
{
    qDebug() << "FUNÇÕES GERADAS COM SUCESSO!";
}

OutArgument *Functions::soma(InArgument * const inArguments ,Action * const action)
{
    Q_UNUSED(action)

    this->getVariable("Number1")->setAttribute(StateVariable::value, (*inArguments)["Numero1"]);
    this->getVariable("Number2")->setAttribute(StateVariable::value, (*inArguments)["Numero2"]);

    int result = this->getVariable("Number1")->getAttribute(StateVariable::value).toInt() + this->getVariable("Number2")->getAttribute(StateVariable::value).toInt();
    this->getVariable("Resultado")->setAttribute(StateVariable::Value,
                                                  result);
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("OutNumber", this->getVariable("Resultado")->getAttribute(StateVariable::Value));

    return outArgs;
}

OutArgument *Functions::subtracao(InArgument * const inArguments, Action * const action)
{
    Q_UNUSED(action)

    this->getVariable("Number1")->setAttribute(StateVariable::value, (*inArguments)["Numero1"]);
    this->getVariable("Number2")->setAttribute(StateVariable::value, (*inArguments)["Numero2"]);

    int result = this->getVariable("Number1")->getAttribute(StateVariable::value).toInt() - this->getVariable("Number2")->getAttribute(StateVariable::value).toInt();
    this->getVariable("Resultado")->setAttribute(StateVariable::Value,
                                                  result);
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("OutNumber", this->getVariable("Resultado")->getAttribute(StateVariable::Value));

    return outArgs;
}

OutArgument *Functions::multiplicacao(InArgument * const inArguments, Action * const action)
{
    Q_UNUSED(action)

    this->getVariable("Number1")->setAttribute(StateVariable::value, (*inArguments)["Numero1"]);
    this->getVariable("Number2")->setAttribute(StateVariable::value, (*inArguments)["Numero2"]);

    int result = this->getVariable("Number1")->getAttribute(StateVariable::value).toInt() * this->getVariable("Number2")->getAttribute(StateVariable::value).toInt();
    this->getVariable("Resultado")->setAttribute(StateVariable::Value,
                                                  result);
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("OutNumber", this->getVariable("Resultado")->getAttribute(StateVariable::Value));

    return outArgs;
}

OutArgument *Functions::divisao(InArgument * const inArguments, Action * const action)
{
    Q_UNUSED(action)

    this->getVariable("Number1")->setAttribute(StateVariable::value, (*inArguments)["Numero1"]);
    this->getVariable("Number2")->setAttribute(StateVariable::value, (*inArguments)["Numero2"]);

    int result = this->getVariable("Number1")->getAttribute(StateVariable::value).toInt() / this->getVariable("Number2")->getAttribute(StateVariable::value).toInt();
    this->getVariable("Resultado")->setAttribute(StateVariable::Value,
                                                  result);
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("OutNumber", this->getVariable("Resultado")->getAttribute(StateVariable::Value));

    return outArgs;
}
