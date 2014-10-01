/*
 * ApplicationUI.hpp
 *
 *  Created on: 09/09/2013
 *      Author: daniel
 */

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ArrayDataModel>

#include "ActiveFrameQML.hpp"
#include "ViewController.hpp"
#include "ActionController.hpp"
#include "model/ExecutableAction.hpp"
#include "applicationinfo.hpp"

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {}
};


#endif /* ApplicationUI_HPP_ */
