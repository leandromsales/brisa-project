/*
 * ActiveFrame.h
 *
 *  Created on: Apr 2, 2013
 *      Author: wbarichak
 */

#ifndef ACTIVEFRAMEQML_H_
#define ACTIVEFRAMEQML_H_

#include <QObject>
#include <bb/cascades/Label>
#include <bb/cascades/SceneCover>

using namespace bb::cascades;

class ActiveFrameQML: public SceneCover {
    Q_OBJECT

public:
    ActiveFrameQML(QObject *parent=0);

};

#endif /* ACTIVEFRAMEQML_H_ */
