/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

#include <bb/cascades/ArrayDataModel>

#include "upnp/controlpoint/Device"
#include "upnp/controlpoint/ControlPoint"

namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
    }
}

class QTranslator;

/*!
 * @brief Application object
 *
 *
 */

using namespace brisa::upnp::controlpoint;

class ApplicationUI : public QObject {
    Q_OBJECT
    Q_PROPERTY(bb::cascades::ArrayDataModel * dataModel READ dataModel NOTIFY dataModelChanged FINAL)
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() { }
    Q_INVOKABLE bb::cascades::ArrayDataModel *dataModel();
    Q_INVOKABLE void bDebug(const QVariant &variant);
    Q_INVOKABLE void getTarget(const QString &udn);
    Q_INVOKABLE void setTarget(const QString &udn, const bool &newState);
    Q_INVOKABLE QVariantMap getDeviceInformations(const QString &udn);
signals:
    void dataModelChanged();
    void resultGetTargetChanged(bool);
private slots:
    void onSystemLanguageChanged();
    void deviceFoundDump(brisa::upnp::controlpoint::Device*);
    void removeDevice(QString);
    void serviceCall(OutArgument, QString);
    void requestError(QString,QString);
private:
    int findDeviceByUDN(const QString &udn);
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    brisa::upnp::controlpoint::Service *m_service;
    brisa::upnp::controlpoint::ControlPoint *m_controlPoint;
    QList<QSharedPointer<brisa::upnp::controlpoint::Device> > m_devicesList;
    bb::cascades::ArrayDataModel * m_dataModel;
};

#endif /* ApplicationUI_HPP_ */
