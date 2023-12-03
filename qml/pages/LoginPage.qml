/*
  Copyright (C) 2016 Petr Vytovtov
  Contact: Petr Vytovtov <osanwe@protonmail.ch>
  All rights reserved.

  This file is part of Kat.

  Kat is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kat is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kat.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0

Page {
    id: loginPage

    Notification {
        id: loginNotification
        category: "harbour-kat"
    }

    Label {
        id: label1
        text: qsTr("Email/Phone:")
        color: Theme.primaryColor
    }

    TextField {
        id: textField1
        width: parent.width
        anchors.top: label1.bottom
        color: Theme.primaryColor

        EnterKey.enabled: false
    }

    Label {
        id: label2
        anchors.top: textField1.bottom
        text: qsTr("Password:")
        color: Theme.primaryColor
    }

    TextField {
        id: textField2
        width: parent.width
        anchors.top: label2.bottom
        visible: true
        color: Theme.primaryColor

        EnterKey.enabled: false
        echoMode: TextInput.Password
    }

    Label {
        id: label3
        anchors.top: textField2.bottom
        text: qsTr("Code:")
        visible: false
        color: Theme.primaryColor
    }

    TextField {
        id: textField3
        width: parent.width
        anchors.top: label3.bottom
        color: Theme.primaryColor
        visible: false
        EnterKey.enabled: false
        text: ""
    }

    Button {
        id: enterButton
        anchors.top: textField3.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: qsTr("Login")
        onClicked: {
            vksdk.auth.tryToGetAccessToken(textField1.text+ " "+ textField2.text, textField3.text)
            //textField1.text = ""
            //textField2.text = ""
            //enterButton.enabled = false
        }
    }

    Connections {
        target: vksdk.auth
        onAuthorized: {
            vksdk.setAccessTocken(accessToken)
            vksdk.setUserId(userId)
            settings.setAccessToken(accessToken)
            settings.setUserId(userId)
            pageContainer.replace(Qt.resolvedUrl("MainMenuPage.qml"))
            loginNotification.previewBody = qsTr("Logged to vk.com with Kat")
            loginNotification.publish()
        }

        onError: {
            enterButton.enabled = true
            loginNotification.previewBody = errorMessage
            loginNotification.publish()
        }

        onCoderequired: {
           label3.visible = true;
           textField3.visible = true
        }
    }
}

