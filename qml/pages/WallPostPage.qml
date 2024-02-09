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
import QtGraphicalEffects 1.0
import Sailfish.Silica 1.0

import "../views"

Page {
    id: profilePage

    property var name
    property var wallpost

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: header.height + content.height

        PullDownMenu {

            MenuItem {
                text: qsTr("Share")
                onClicked: {
                    var sourceId = wallpost.sourceId === 0 ? wallpost.fromId : wallpost.sourceId
                    pageStack.push(Qt.resolvedUrl("RepostPage.qml"), { sourceId: sourceId,
                                                                       postId: wallpost.id })
                }
            }

            MenuItem {
                text: qsTr("Like")
                onClicked: {
//                    var sourceId = wallpost.sourceId === 0 ? wallpost.fromId : wallpost.sourceId
                    vksdk.likes.addPost(wallpost.toId, wallpost.id)
                }
            }
        }

        PageHeader {
            id: header
            title: name
        }

        Column {
            id: content
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.rightMargin: Theme.horizontalPageMargin
            spacing: Theme.paddingMedium

            Loader {
                property var _wallpost: wallpost
                property var _repost: wallpost.repost
                property bool isFeed: false
                width: parent.width
                active: true
                source: "../views/WallPostView.qml"
            }

            Rectangle {
                color:Theme.darkPrimaryColor
                width: parent.width
                height:2
            }

            Column {
                width: parent.width
                spacing: Theme.paddingMedium

                Repeater {
                    model: vksdk.commentsModel
                    delegate: ListItem {
                        width: parent.width
                        //height: childrenRect.height
                        contentHeight: commentContent.height
                        property real avatarSize : Theme.iconSizeMedium
                        Image {
                            id: commentAvatar
                            anchors.top: parent.top
                            anchors.left: parent.left

                            width: avatarSize
                            height: avatarSize
                            source: avatarSource

                            layer.enabled: true
                                    layer.effect: OpacityMask {
                                        maskSource: commentAvatarMask
                                    }
                        }
                        Rectangle {
                                id: commentAvatarMask
                                width: avatarSize
                                height: avatarSize
                                radius: avatarSize
                                visible: false
                        }
                        Column {
                            id: commentContent
                            anchors {
                                left: commentAvatar.right
                                right: parent.right
                                leftMargin: Theme.paddingMedium
                            }
                            Row {
                                width: parent.width
                                spacing: Theme.paddingSmall
                                Label {
                                    id: commentUserName
                                    color: Theme.highlightColor
                                    font.pixelSize: Theme.fontSizeSmall
                                    wrapMode: Text.WordWrap
                                    text: title
                                }
                                Image {
                                    id: commnetLikeIcon
                                    width: Theme.fontSizeTiny
                                    height: Theme.fontSizeTiny
                                    source: "image://theme/icon-s-like?" +
                                            (userLiked ? Theme.secondaryHighlightColor : Theme.secondaryColor)
                                }

                                Label {
                                    id: commentLikeCounter
                                    color: Theme.secondaryColor
                                    font.pixelSize: Theme.fontSizeTiny
                                    text: likeCount
                                }
                            }
                            Label {
                                width: parent.width
                                font.pixelSize: Theme.fontSizeSmall
                                wrapMode: Text.WordWrap
                                text: commentText
                            }

                        }
                        menu: ContextMenu {

                            MenuItem {
                                text: qsTr("Like")
                                onClicked: {
                                    vksdk.likes.addComment(wallpost.toId, commentId)
                                    commentLikeCounter.text = parseInt(commentLikeCounter.text) +1
                                    commnetLikeIcon.source = "image://theme/icon-s-like?" + Theme.secondaryHighlightColor
                                }
                            }
                        }
                        Component.onCompleted: console.log(avatarSource, commentText)
                    }
                }
            }

            TextField {
                width: parent.width
                placeholderText: qsTr("Your comment")
                label: qsTr("Your comment")
                visible: wallpost.canComment

                EnterKey.enabled: text.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: {
//                    var sourceId = wallpost.sourceId === 0 ? wallpost.fromId : wallpost.sourceId
                    vksdk.wall.createComment(wallpost.toId, wallpost.id, text)
                    text = ""
                }
            }
        }

        VerticalScrollDecorator {}
    }

    Connections {
        target: vksdk
        onCommentCreated: {
            vksdk.commentsModel.clear()
//            var sourceId = wallpost.toId === 0 ? wallpost.fromId : wallpost.toId
            vksdk.wall.getComments(wallpost.toId, wallpost.id)
        }
    }

    onStatusChanged: if (status === PageStatus.Active) {
                         vksdk.commentsModel.clear()
//                         var sourceId = wallpost.sourceId === 0 ? wallpost.fromId : wallpost.sourceId
                         vksdk.wall.getComments(wallpost.toId, wallpost.id)
//                         pageStack.pushAttached(Qt.resolvedUrl("AudioPlayerPage.qml"))
                     }
}

