#ifndef GETGAME_H
#define GETGAME_H

#include <BrisaAction>
#include <BrisaService>

using namespace Brisa;

class GetGame : public BrisaAction
{
public:
    GetGame(BrisaService *service);

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // GETGAME_H
