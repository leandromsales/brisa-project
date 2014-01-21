/*
 * ActiveFrameQML.hpp
 *
 *  Created on: 29/09/2013
 *      Author: daniel
 */

#ifndef ACTIVEFRAMEQML_H_
#define ACTIVEFRAMEQML_H_

#include <QObject>
#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>
#include <bb/cascades/QmlDocument>

class ActiveFrameQML: public bb::cascades::SceneCover {
    Q_OBJECT
public:
    ActiveFrameQML(QObject *parent=0);
};

#endif /* ACTIVEFRAMEQML_H_ */
