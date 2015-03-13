/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import "../views"
import "../js/auth.js" as AuthJS
import "../js/storage.js" as StorageJS
import "../js/api/messages.js" as MessagesAPI
import "../js/api/users.js" as UsersAPI


Page {
    id: startPage

    property int chatsCounter: 0
    property int dialogsOffset: 0

    function initialize() {
//        StorageJS.initDatabase()
        if (!StorageJS.readSettingsValue("user_id")) {
            pageStack.push(Qt.resolvedUrl("LoginPage.qml"))
        } else {
            dialogsOffset = 0
            chatsCounter = 0
            loadingDialogsIndicator.running = true
            messagesList.footerItem.visible = false
            messagesList.model.clear()  // TODO: Oh, really?!
            MessagesAPI.getDialogs(dialogsOffset)
        }
    }

    function doMainMenuItem() {
        if (StorageJS.readSettingsValue("user_id")) {
            console.log("Refreshing")
            loadingDialogsIndicator.running = true
            messagesList.footerItem.visible = false
            messagesList.model.clear()  // TODO: Oh, really?!
            chatsCounter = 0
            dialogsOffset = 0
            MessagesAPI.getDialogs(dialogsOffset)
        } else {
            console.log("You have to sign in")
        }
    }

    function formDialogsList(io, title, message, dialogId, readState, isChat) {
        console.log(readState)
        message = message.replace(/<br>/g, " ")
        messagesList.model.append({ io: io,
                                      avatarSource: "image://theme/icon-cover-message",
                                      nameOrTitle: title,
                                      lastMessage: message,
                                      dialogId: dialogId,
                                      readState: readState,
                                      isOnline: false,
                                      isChat: isChat })
    }

    function updateDialogsList(index, avatarURL, fullname, online) {
        while (messagesList.model.get(parseInt(index, 10)+chatsCounter+dialogsOffset).isChat) chatsCounter += 1
        messagesList.model.setProperty(parseInt(index, 10)+chatsCounter+dialogsOffset, "avatarSource", avatarURL)
        messagesList.model.setProperty(parseInt(index, 10)+chatsCounter+dialogsOffset, "nameOrTitle", fullname)
        messagesList.model.setProperty(parseInt(index, 10)+chatsCounter+dialogsOffset, "isOnline", online)
    }

    function stopBusyIndicator() {
        messagesList.footerItem.visible = true
        loadingDialogsIndicator.running = false
    }

    BusyIndicator {
        id: loadingDialogsIndicator
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: true
    }

    SilicaListView {
        id: messagesList
        anchors.fill: parent
        model: ListModel {}
        delegate: DialogItem { id: dialogItem }

        PullDownMenu {
            MenuItem {
                id: aboutItem
                text: "О программе"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
//            MenuItem {
//                id: settingsItem
//                text: "Настройки"
//                onClicked: doMainMenuItem()
//            }
            MenuItem {
                id: newMessageItem
                text: "Новое сообщение"
                onClicked: pageStack.push(Qt.resolvedUrl("NewMessagePage.qml"))
            }
            MenuItem {
                id: mainMenuItem
                text: "Обновить"
                onClicked: doMainMenuItem()
            }
        }

        header: PageHeader {
            title: "Сообщения"
        }

        footer: Button {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width / 3 * 2
            text: "Загрузить больше"
            onClicked: { loadingDialogsIndicator.running = true; dialogsOffset = dialogsOffset + 20; chatsCounter = 0; MessagesAPI.getDialogs(dialogsOffset) }
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: {
        console.log("MessagesPage status = " + status)
        if (status === PageStatus.Active) initialize()
    }

    Component.onCompleted: StorageJS.initDatabase()
}


