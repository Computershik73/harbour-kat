/*
  Copyright (C) 2015 Petr Vytovtov
  Contact: Petr Vytovtov <osanwe@protonmail.ch>
  All rights reserved.

  This file is part of vkFish.

  vkFish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Foobar is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import "../views"
import "../js/api/messages.js" as MessagesAPI

Page {
    id: dialogPage

    property string fullname
    property int dialogId
    property bool isChat

    property int messagesOffset: 0

    function sendMessage() {
        MessagesAPI.sendMessage(isChat, dialogId, messageInput.text, false)
        messages.model.clear()
        messageInput.text = ""
        messagesOffset = 0
    }

    function formMessagesList(io, readState, text) {
        text = text.replace(/<br>/g, "\n")
        messages.model.insert(0, {io: io, readState: readState, message: text} )
    }

    function scrollMessagesToBottom() {
        if (messagesOffset === 0)
            messages.positionViewAtEnd()
        else
            messages.positionViewAtIndex(49, ListView.Beginning)
    }

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: dialogTitle
            title: fullname
        }

        SilicaListView {
            id: messages
            anchors.fill: parent
            anchors.topMargin: dialogTitle.height
            anchors.bottomMargin: messageInput.height
            clip: true

            model: ListModel {}

            header: Button {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width / 3 * 2
                text: "Загрузить больше"
                onClicked: {
                    messagesOffset = messagesOffset + 50;
                    MessagesAPI.getHistory(isChat, dialogId, messagesOffset)
                }
            }

            delegate: MessageItem {}

            VerticalScrollDecorator {}
        }

        TextArea {
            id: messageInput
            width: parent.width
            anchors.bottom: parent.bottom
            placeholderText: "Сообщение:"
            label: "Сообщение:"

            EnterKey.enabled: text.length > 0
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: sendMessage()
        }

        PushUpMenu {

            MenuItem {
                text: "Обновить"
                onClicked: {
                    messages.model.clear()
                    messagesOffset = 0
                    MessagesAPI.getHistory(isChat, dialogId, messagesOffset)
                }
            }

            MenuItem {
                text: "Отметить прочитанным"
                onClicked: {
                    messages.model.clear()
                    messagesOffset = 0
                    MessagesAPI.markDialogAsRead(isChat, dialogId)
                }
            }
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: MessagesAPI.getHistory(isChat, dialogId, messagesOffset)
}
