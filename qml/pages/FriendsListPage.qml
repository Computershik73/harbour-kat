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
    id: friendsListPage

    property var userId
    property var type // 1 - all; 2 - online; 3 - mutual

    SilicaListView {
        id: friendsListView
        anchors.fill: parent

        PullDownMenu {

            MenuItem {
                text: qsTr("Mutual friends")
                visible: type !== 3 && userId !== settings.userId()
                onClicked: {
                    type = 3
                    vksdk.friendsListModel.clear()
                    vksdk.friends.getMutual(userId)
                }
            }

            MenuItem {
                text: qsTr("Online friends")
                visible: type !== 2
                onClicked: {
                    type = 2
                    vksdk.friendsListModel.clear()
                    vksdk.friends.getOnline(userId)
                }
            }

            MenuItem {
                text: qsTr("All friends")
                visible: type !== 1
                onClicked: {
                    type = 1
                    vksdk.friendsListModel.clear()
                    vksdk.friends.get(userId)
                }
            }
        }

        model: vksdk.friendsListModel

        header: PageHeader {
            title: type === 1 ? qsTr("Friends") : type === 2 ? qsTr("Online friends") :
                                                               qsTr("Mutual friends")
        }

        delegate: UserListItem {
            isUser: true
            avatarSource: avatar
            onlineStatus: online
            titleText: name
            bodyText: status

            onClicked: pageContainer.push(Qt.resolvedUrl("ProfilePage.qml"), { profileId: id })
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: if (status === PageStatus.Active) {
                         pageStack.pushAttached(Qt.resolvedUrl("AudioPlayerPage.qml"))
                         vksdk.friendsListModel.clear()
                         switch (type) {
                         case 1:
                             vksdk.friends.get(userId)
                             break;
                         case 2:
                             vksdk.friends.getOnline(userId)
                             break;
                         case 3:
                             vksdk.friends.getMutual(userId)
                             break;
                         default:
                             break;
                         }
                     }
}

