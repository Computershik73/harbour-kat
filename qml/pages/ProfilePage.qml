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
    id: profilePage

    property var profileId
    property var profile
    property bool isBanned: false

    ViewPlaceholder {
        id: systemMessage
        enabled: false
    }

    SilicaFlickable {
        id: userDataView
        anchors.fill: parent
        contentHeight: content.height + header.height

        PullDownMenu {

            MenuItem {
                visible: profile.domain.length > 0
                text: qsTr("Open in browser")
                onClicked: Qt.openUrlExternally("https://m.vk.com/" + profile.domain)
            }

            MenuItem {
                text: isBanned ? qsTr("Remove from blacklist") : qsTr("Add to blacklist")
                visible: profileId !== settings.userId()
                onClicked: {
                    if (isBanned) vksdk.account.unbanUser(profileId)
                    else vksdk.account.banUser(profileId)
                }
            }

//            MenuItem {
//                text: profile.isFavorite ? qsTr("Remove from favorites") : qsTr("Add to favorites")
//                onClicked: console.log("...favoriting...")
//            }

//            MenuItem {
//                text: {
//                    switch (profile.friendStatus) {
//                    case 0:
//                        if (!profile.canSendFriendRequest) visible = false
//                        return qsTr("Add to friends")
//                    case 2:
//                        return qsTr("Add to friends")
//                    case 1:
//                        return qsTr("Cancel friend request")
//                    case 3:
//                        return qsTr("Remove from friends")
//                    }
//                }
//                onClicked: console.log("...friending...")
//            }

            MenuItem {
                visible: profile.canWritePrivateMessage
                text: qsTr("Go to dialog")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("DialogPage.qml"), { chat: false,
                                                                       historyId: profile.id })
                }
            }
        }

        PageHeader {
            id: header
            title: profile.firstName + " " + profile.lastName
            description: {
                if (!profile.online) {
                    var date = new Date(profile.lastSeenTime * 1000)
                    var day = "0" + date.getDate()
                    var month = "0" + (date.getMonth() + 1)
                    var year = date.getFullYear()
                    var hours = "0" + date.getHours()
                    var minutes = "0" + date.getMinutes()
                    var datetime = day.substr(-2) + "." + month.substr(-2) + "." + year + " " +
                            hours.substr(-2) + ":" + minutes.substr(-2)
                    var platform = ""
                    switch (profile.lastSeenPlatform) {
                    case 1:
                        platform = "Mobile"
                        break;
                    case 2:
                        platform = "iPhone"
                        break;
                    case 3:
                        platform = "iPad"
                        break;
                    case 4:
                        platform = "Android"
                        break;
                    case 5:
                        platform = "Windows Phone"
                        break;
                    case 6:
                        platform = "Windows 8"
                        break;
                    case 7:
                        platform = "Web"
                        break;
                    }
                    switch(Qt.locale().name.substring(0,2)) {
                        case "ru":
                            return ((profile.sex === 1)? qsTr("Last seen female: "):qsTr("Last seen: ")) + datetime + " (" + platform + ")"
                        default:
                            return qsTr("Last seen: ") + datetime + " (" + platform + ")"
                    }

                } else return ""
            }

            Switch {
                anchors.verticalCenter: parent.verticalCenter
                automaticCheck: false
                checked: profile.online
            }
        }

//        Image {
//            anchors.horizontalCenter: parent.horizontalCenter
//            width: Theme.iconSizeSmall
//            height: Theme.iconSizeSmall
//            source: "image://theme/icon-s-installed"
//            visible: profile.verified
//        }

        Column {
            id: content
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 2 * Theme.horizontalPageMargin
                spacing: Theme.paddingLarge

                Image {
                    width: Theme.iconSizeExtraLarge
                    height: Theme.iconSizeExtraLarge
                    source: profile.photo200

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageContainer.push(Qt.resolvedUrl("ImageViewPage.qml"),
                                                  { imagesModel: [profile.photoMaxOrig] })
                    }
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - Theme.iconSizeExtraLarge - Theme.paddingLarge
                    color: Theme.secondaryColor
                    maximumLineCount: 4
                    wrapMode: Text.WordWrap
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: profile.status
                }
            }

            CollapsedView {
                id: profileInfo
                width: parent.width

                bdate: profile.bdate
                relation: profile.relation
                relationPartnerId: profile.relationPartnerId
                relationPartnerName: profile.relationPartnerFirstName + " " + profile.relationPartnerLastName
                occupationId: profile.occupationId
                occupationType: profile.occupationType
                occupationName: profile.occupationName

                sex: profile.sex
                city: profile.cityTitle
                country: profile.countryTitle
                homeTown: profile.homeTown
                mobilePhone: profile.contactsMobilePhone
                homePhone: profile.contactsHomePhone
                site: profile.site
            }

            MoreButton {
                id: friendsButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Friends")
                counter: profile.counterFriends
                visible: profile.counterFriends > 0

                onClicked: pageContainer.push(Qt.resolvedUrl("FriendsListPage.qml"),
                                              { userId: profile.id, type: 1 })
            }

            MoreButton {
                id: photosButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Photos")
                counter: profile.counterPhotos
                visible: profile.counterPhotos > 0

                onClicked: pageContainer.push(Qt.resolvedUrl("PhotoAlbumPage.qml"), { ownerId: profile.id })
            }

            MoreButton {
                id: videosButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Videos")
                counter: profile.counterVideos
                visible: profile.counterVideos > 0
            }

            MoreButton {
                id: audiosButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Audios")
                counter: profile.counterAudios
                visible: profile.counterAudios > 0
                onClicked: {
                                    vksdk.audios.get(profileId)
                                    pageContainer.navigateForward()
                                }
            }

            MoreButton {
                id: groupsButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Groups")
                counter: profile.counterGroups
                visible: profile.counterGroups > 0

                onClicked: pageContainer.push(Qt.resolvedUrl("GroupsListPage.qml"),
                                              { userId: profile.id })
            }

            MoreButton {
                id: notesButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Notes")
                counter: profile.counterNotes
                visible: profile.counterNotes > 0
            }

            MoreButton {
                id: wallButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Wall")
                counter: vksdk.wallModel.count
                visible: vksdk.wallModel.count > 0
                isopen: true
            }

            ListView {
                id: walllist
                width: parent.width
                height: contentHeight
                model: vksdk.wallModel
                visible: vksdk.wallModel.count > 0
                delegate: Component {

                    Loader {
                        property var _avatarSource: avatarSource
                        property var _title: title
                        property var __wallpost: wallpost
                        width: parent.width
                        source: "../views/WallItem.qml"
                    }
                }
            }
        }

        PushUpMenu {
            visible: walllist.count !== vksdk.wallModel.count

            MenuItem {
                text: qsTr("Load more")
                onClicked: vksdk.wall.get(profileId, walllist.count)
            }
        }

        VerticalScrollDecorator {}
    }

    Connections {
        target: vksdk
        onGotProfile: {
            if (profileId === user.id) {
                profile = user
                isBanned = user.blacklisted
                if (profile.deactivated.length > 0) {
                    userDataView.visible = false
                    systemMessage.enabled = true
                    if (profile.deactivated === "deleted") systemMessage.text = qsTr("The user is deleted")
                    else if (profile.deactivated === "banned") systemMessage.text = qsTr("The user is baned")
                }
            }
        }
        onBanSettingChanged: isBanned = banned
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("AudioPlayerPage.qml"))
            if (vksdk.wallModel.id !== profileId) {
                vksdk.wallModel.clear()
                vksdk.wallModel.id = profileId
                vksdk.wall.get(profileId)
            }
        }
    }

    Component.onCompleted: {
        vksdk.users.getUserProfile(profileId)
    }
}
