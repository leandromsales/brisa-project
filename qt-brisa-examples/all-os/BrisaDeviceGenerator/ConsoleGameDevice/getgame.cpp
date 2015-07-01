#include "getgame.h"

GetGame::GetGame(BrisaService *service) : BrisaAction("GetGame", service)
{
}

QMap<QString, QString> GetGame::run(const QMap<QString, QString> &inArguments)
{
    Q_UNUSED(inArguments);
    QMap<QString, QString> outArgs;
    outArgs.insert("Game", this->getStateVariable("Game")->
                   getAttribute(BrisaStateVariable::Value));
    return outArgs;
}
