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

import bb.cascades 1.2

TabbedPane {
    showTabsOnActionBar: true
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: "About"
                imageSource: "asset:///images/ic_info.png"
                onTriggered: {
                    aboutDefinition.createObject().open()
                }
            }
        ]
    }
    attachedObjects: ComponentDefinition {
        id: aboutDefinition
        About {
        }
    }
    onActiveTabChanged: {
        if (activeTab.title == "Device Informations")
            informations.setDeviceUdn(invokeActions.getDeviceUdn())
    }
    Tab {
        title: "Invoke Actions"
        InvokeActions {
            id: invokeActions
        }
    }
    Tab {
        title: "Device Informations"
        Informations {
            id: informations
        }
    }
}