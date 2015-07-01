#ifndef SETGAME_H
#define SETGAME_H

#include <BrisaAction>
#include <BrisaService>

using namespace Brisa;

class SetGame : public BrisaAction
{
public:
    SetGame(BrisaService *service);

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // SETGAME_H
