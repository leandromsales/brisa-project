#include "activeframeqml.h"

#include <bb/cascades/SceneCover>
#include <bb/cascades/Container>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

ActiveFrameQML::ActiveFrameQML(QObject *parent)
    : SceneCover(parent)
{
    QmlDocument *qml = QmlDocument::create("asset:///AppCover.qml")
            .parent(parent);
    Container *mainContainer = qml->createRootObject<Container>();
    setContent(mainContainer);
}
