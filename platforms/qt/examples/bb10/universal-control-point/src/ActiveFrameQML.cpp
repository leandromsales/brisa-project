/*
 * ActiveFrameQML.cpp
 *
 *  Created on: 29/09/2013
 *      Author: daniel
 */

#include "ActiveFrameQML.hpp"

using namespace bb::cascades;

ActiveFrameQML::ActiveFrameQML(QObject *parent)
    : SceneCover(parent)
{
	QmlDocument *qml = QmlDocument::create("asset:///qml/AppCover.qml")
            .parent(parent);
	Container *mainContainer = qml->createRootObject<Container>();
    setContent(mainContainer);
}
