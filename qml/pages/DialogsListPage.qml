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

import "../views"

Page {
    id: dialogsListPage

    SilicaListView {
        id: dialogsList
        anchors.fill: parent
        clip: true

        model: vksdk.dialogsListModel

//        PullDownMenu {

//            MenuItem {
//                text: qsTr("New message")
//            }
//        }

        header: PageHeader {
            title: qsTr("Dialogs")
        }

        delegate: BackgroundItem {
            id: dialogItem
            width: parent.width
            height: Theme.itemSizeMedium

            Switch {
                id: unreadStatus
                anchors.horizontalCenter: parent.left
                anchors.verticalCenter: parent.verticalCenter
                automaticCheck: false
                checked: unread
                visible: unread && !isOut
            }

            DialogImage {
                id: dialogAvatar
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: Theme.horizontalPageMargin
                width: Theme.iconSizeMedium
                height: Theme.iconSizeMedium
                src: avatar
            }

            Column {
                anchors.left: dialogAvatar.right
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: Theme.paddingMedium
                anchors.rightMargin: Theme.horizontalPageMargin

                Row {
                    width: parent.width
                    height: childrenRect.height
                    spacing: Theme.paddingMedium

                    Switch {
                        id: isOnline
                        height: name.height
                        width: height
                        automaticCheck: false
                        checked: online
                        visible: !isChat
                    }

                    Label {
                        id: name
                        width: parent.width - isOnline.width - Theme.paddingMedium
                        color: dialogItem.highlighted || (unread && !isOut) ? Theme.highlightColor :
                                                                              Theme.primaryColor
                        font.bold: true
                        truncationMode: TruncationMode.Fade
                        text: dialogTitle
                    }
                }

                Label {
                    width: parent.width
                    color: dialogItem.highlighted || unread ? Theme.secondaryHighlightColor :
                                                              Theme.secondaryColor
                    linkColor: dialogItem.highlighted ? Theme.primaryColor : Theme.highlightColor
                    truncationMode: TruncationMode.Fade
                    maximumLineCount: 1
                    text: preview.replace('\n', ' ')
                }
            }

            onClicked: pageContainer.push(Qt.resolvedUrl("DialogPage.qml"), { chat: isChat,
                                                                              historyId: id })

            Component.onCompleted:
                if (index == vksdk.dialogsListModel.size-1) vksdk.messages.getDialogs(vksdk.dialogsListModel.size)
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: if (status === PageStatus.Active) pageStack.pushAttached(Qt.resolvedUrl("AudioPlayerPage.qml"))

    Component.onCompleted: vksdk.messages.getDialogs()
}

