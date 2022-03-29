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
;import org.nemomobile.notifications 1.0

Page {
    id: loginPage

    Notification {
        id: loginNotification
        category: "harbour-kat"
    }

    //SilicaWebView {
  //      id: loginWebView

 //       anchors.fill: parent
  //      url: vksdk.auth.authUrl

   //     onUrlChanged: vksdk.auth.tryToGetAccessToken(url)
   // }

    TextField {
        id: loginWebView
        width: parent.width - parent.spacing
        placeholderText: qsTr("Логин и пароль через пробел")
        label: qsTr("Логин и пароль через пробел")



        EnterKey.enabled: text.length > 0
        EnterKey.iconSource: "image://theme/icon-m-enter-accept"
        EnterKey.onClicked: {
            vksdk.auth.tryToGetAccessToken(loginWebView.text)
            loginWebView.text = ""
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
            loginNotification.previewBody = errorMessage
            loginNotification.publish()
            //loginWebView.url = vksdk.auth.authUrl
        }
    }
}

