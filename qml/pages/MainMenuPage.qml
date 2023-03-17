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

Page {
    id: mainMenuPage
    
    enum Category {
        Profile,
        Messages,
        Friends,
        Groups,
        Photos,
        Music
    }

    property var menuItems: [
        { itemText: qsTr("My profile"), counter: 0 },
        { itemText: qsTr("Messages"),   counter: 0 },
        { itemText: qsTr("Friends"),   counter: 0 },
        { itemText: qsTr("Groups"),   counter: 0 },
        { itemText: qsTr("Photos"),   counter: 0 },
        { itemText: qsTr("Music"),   counter: 0 },




    ]

    /**
     *  The function does initial actions for showing menu and additional information.
     */
    function init() {
        busyIndicator.running = true
        for (var index in menuItems) menuList.model.append(menuItems[index])
        vksdk.stats.trackVisitor()
        vksdk.users.getSelfProfile()
        vksdk.messages.getDialogs()
        vksdk.audios.get()
        vksdk.longPoll.getLongPollServer()

    }



    function openSubPage(menuItemIndex) {
        switch (menuItemIndex) {
        case 0: // Self profile page
            pageContainer.push(Qt.resolvedUrl("ProfilePage.qml"), { profileId: settings.userId() })
            break
            // case 1: // Newsfeed page
            //     pageContainer.push(Qt.resolvedUrl("NewsfeedPage.qml"))
            //     break
        case 1: // Dialogs page
            pageContainer.push(Qt.resolvedUrl("DialogsListPage.qml"))
            break
        case 2: // Friends page
            pageContainer.push(Qt.resolvedUrl("FriendsListPage.qml"), { userId: settings.userId(),
                                   type:   1 })
            break
        case 3: // Groups page
            pageContainer.push(Qt.resolvedUrl("GroupsListPage.qml"), { userId: settings.userId() })
            break
        case 4: // Photos page
            pageContainer.push(Qt.resolvedUrl("PhotoAlbumPage.qml"), { ownerId: settings.userId() })
            break
        case 5: //Music page
            pageContainer.push(Qt.resolvedUrl("AudioPlayerPage.qml"))
            break
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: false
    }

    SilicaListView {
        id: menuList
        anchors.fill: parent
        model: ListModel {}

        PullDownMenu {

            MenuItem {
                text: qsTr("About")
                onClicked: pageContainer.push(Qt.resolvedUrl("AboutPage.qml"))
            }


            MenuItem {
                text: qsTr("Settings")
                onClicked: pageContainer.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        header: PageHeader {}

        delegate: BackgroundItem {
            id: menuItem
            width: parent.width
            height: Theme.itemSizeSmall

            property var item: model.modelData ? model.modelData : model

            Label {
                anchors.left: parent.left
                anchors.right: menuItemCounter.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: Theme.horizontalPageMargin
                color: menuItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                text: item.itemText
            }

            Label {
                id: menuItemCounter
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Theme.fontSizeLarge
                color: Theme.highlightColor
                text: item.counter === 0 ? "" : item.counter
            }

            onClicked: openSubPage(index)
        }
    }

    Connections {
        target: vksdk.longPoll
        onUnreadDialogsCounterUpdated: menuList.model.setProperty(Page.Category.Messages, "counter", value)
    }

    Connections {
        target: vksdk
        onGotProfile: {
            if (status === PageStatus.Active) {
                busyIndicator.running = false
                menuList.headerItem.title = user.firstName + " " + user.lastName
            }
        }
        onGotUnreadCounter: {
            menuList.model.setProperty(Page.Category.Messages, "counter", value)
        }
        onGotUserAudios: {

            busyIndicator.running = false
            player.setPlaylist(audios, -1)
        }

    }

    Timer {
        id: onlineTimer
        interval: 900000
        repeat: true
        triggeredOnStart: false

        onTriggered: {
            if (!settings.offlineStatus()) vksdk.account.setOnline()
            else stop()
        }
    }

    onStatusChanged: if (status === PageStatus.Active) {
                         pageStack.pushAttached(Qt.resolvedUrl("NewsfeedPage.qml"))
                         // vksdk.dialogsListModel.clear()
                         // vksdk.friendsListModel.clear()
                         // vksdk.groupsListModel.clear()
                         //vksdk.messagesModel.clear()
                         // vksdk.newsfeedModel.clear()
                         // vksdk.wallModel.clear()
                     }

    Component.onCompleted: {
        if (!settings.offlineStatus()) {
            vksdk.account.setOnline()
            onlineTimer.start()
        }
        init()
    }
}

