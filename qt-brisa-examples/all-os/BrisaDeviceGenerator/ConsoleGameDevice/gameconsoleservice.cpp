#include "gameconsoleservice.h"
#include "reset.h"
#include "setgame.h"
#include "getgame.h"

GameConsoleService::GameConsoleService() :
        BrisaService("urn:schemas-upnp-org:service:GameConsole:1",
                     "GameConsole", "/GameConsole/GameConsole-scpd.xml",
                     "/GameConsole/control", "/GameConsole/eventSub")
{
    setDescriptionFile(
            "../BrisaDeviceGenerator/ConsoleGameDevice/GameConsole-scpd.xml");
    addAction(new Reset(this));
    addAction(new SetGame(this));
    addAction(new GetGame(this));
}
