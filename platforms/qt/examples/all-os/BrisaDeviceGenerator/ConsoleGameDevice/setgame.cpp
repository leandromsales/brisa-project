#include "setgame.h"

SetGame::SetGame(BrisaService *service) : BrisaAction("SetGame", service)
{
}

QMap<QString, QString> SetGame::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);
    this->getStateVariable("Game")->
            setAttribute(BrisaStateVariable::Value, inArguments["Game"]);
    QMap<QString, QString> outArguments;
    return outArguments;
}
