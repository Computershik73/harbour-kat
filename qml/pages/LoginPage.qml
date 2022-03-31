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
//import org.nemomobile.notifications 1.0

Page {
    id: loginPage

 /*   Notification {
        id: loginNotification
        category: "harbour-kat"
    }*/

    //SilicaWebView {
  //      id: loginWebView

 //       anchors.fill: parent
  //      url: vksdk.auth.authUrl

   //     onUrlChanged: vksdk.auth.tryToGetAccessToken(url)
   // }




    Label {
      id: label1
     text: "Логин: "
     color: Theme.primaryColor
    visible: true
    }




    TextField {
        id: textField1
        width: parent.width
        anchors.top: label1.bottom
        //placeholderText: qsTr("Логин и пароль через пробел")
        //label: qsTr("Логин и пароль через пробел")
        visible: true
        color: Theme.primaryColor

        //description : qsTr("Логин и пароль через пробел")



        EnterKey.enabled: false
        /*EnterKey.iconSource: "image://theme/icon-m-enter-accept"
        EnterKey.onClicked: {
            vksdk.auth.tryToGetAccessToken(textField1.text)
            loginWebView.text = ""
        }*/
    }

    Label {
      id: label2
      anchors.top: textField1.bottom
     text: "Пароль: "
     color: Theme.primaryColor
    visible: true
    }

    TextField {
        id: textField2
        width: parent.width
        anchors.top: label2.bottom
        //placeholderText: qsTr("Логин и пароль через пробел")
       // label: qsTr("Логин и пароль через пробел")
        visible: true
        color: Theme.primaryColor

        //description : qsTr("Логин и пароль через пробел")



        EnterKey.enabled: false
      /*  EnterKey.iconSource: "image://theme/icon-m-enter-accept"
        EnterKey.onClicked: {
            vksdk.auth.tryToGetAccessToken(loginWebView.text)
            loginWebView.text = ""
        }*/
    }

    Button {
            id: enterButton
            anchors.top: textField2.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Войти")
            onClicked: {
                        vksdk.auth.tryToGetAccessToken(textField1.text+ " "+ textField2.text)
                        textField1.text = ""
                        textField2.text = ""
                enterButton.enabled = false
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
           // loginNotification.previewBody = qsTr("Logged to vk.com with Kat")
            //loginNotification.publish()
        }
        onError: {
             enterButton.enabled = true
           // loginNotification.previewBody = errorMessage
           // loginNotification.publish()
            //loginWebView.url = vksdk.auth.authUrl
        }
    }
}

