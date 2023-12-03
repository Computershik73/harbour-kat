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
import Sailfish.Pickers 1.0

import "../views"

Page {
    id: dialogPage


    property bool chat
    property var historyId
    property var profiles

    property string attachmentsList: ""

    SilicaFlickable {
        anchors.fill: parent

        SilicaListView {
            id: messagesListView
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: typingLabel.top
            verticalLayoutDirection: ListView.BottomToTop
            clip: true

            model: vksdk.messagesModel

            delegate: MessageItem {
                anchors.left: parent.left
                anchors.right: parent.right

                userId: fromId
                date: datetime
                isOut: out
                isRead: read
                avatarSource: avatar
                bodyText: body
                photos: photosList
                videos: videosList
                audios: audiosList
                documents: documentsList
                links: linksList
                news: newsList
                geoTile: geoTileUrl
                geoMap: geoMapUrl
                fwdMessages: fwdMessagesList

                Component.onCompleted: {
                    if (index === vksdk.messagesModel.size-1) vksdk.messages.getHistory(historyId, vksdk.messagesModel.size)
                }
            }

            VerticalScrollDecorator {}
        }

        Label {
            id: typingLabel
            anchors.bottom: newmessagerow.top
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            text: qsTr("typing...")
            visible: false
            onVisibleChanged: if (visible) typingLabelTimer.running = true

            Timer {
                id: typingLabelTimer
                interval: 10000
                onTriggered: typingLabel.visible = false
            }
        }



        Row {
            id: newmessagerow
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
//            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.rightMargin: Theme.horizontalPageMargin
            spacing: Theme.paddingMedium

            TextField {
                id: message_text
                width: parent.width - attachButton.width - parent.spacing
                placeholderText: qsTr("Your message")
                label: qsTr("Your message")

                onTextChanged: {
                    if (!typingTimer.running) {
                        typingTimer.running = true
                        vksdk.messages.setActivity(historyId)
                    }
                }

                EnterKey.enabled: text.length > 0 || attachmentsList.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: {
                    vksdk.messages.send(historyId, text, attachmentsList)
                    text = ""
                    attachmentsList = ""
                }
            }


            Image {
                id: attachButton
                width: Theme.iconSizeMedium
                height: Theme.iconSizeMedium
                source: "image://theme/icon-m-attach"

                BusyIndicator {
                    id: attachmentsBusy
                    anchors.fill: parent
                    size: BusyIndicatorSize.Medium
                    running: false
                }

                Label {
                    id: attachmentsCounter
                    anchors.verticalCenter: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: text === "10" ? 0 : Theme.paddingSmall
                    anchors.verticalCenterOffset: Theme.paddingSmall
                    font.bold: true
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.highlightColor
                    text: {
                        var attachmentsCount = attachmentsList.split(',').length - 1
                        return attachmentsCount > 0 ? attachmentsCount : ""
                    }
                }
            }
        }

        PushUpMenu {
            id: attachmentsMenu

            Column {
                width: parent.width
                height: childrenRect.height

               // Row {
                 //   width: parent.width
                   // height: childrenRect.height

                    MenuItem {
                        width: parent.width
                        height: Theme.itemSizeMedium

                        Row {
                            //anchors.centerIn: parent
                            spacing: Theme.paddingMedium

                            Image {
                                width: Theme.iconSizeSmallPlus
                                height: Theme.iconSizeSmallPlus
                                source: "image://theme/icon-m-camera"
                            }

                            Label {
                                anchors.verticalCenter: parent.verticalCenter
                                width: contentWidth
                                font.pixelSize: Theme.fontSizeTiny
                                text: qsTr("Photo")
                            }
                        }

                        onClicked: if (!attachmentsBusy.running) {
                                       var imagePicker = pageStack.push("Sailfish.Pickers.ImagePickerPage")
                                       imagePicker.selectedContentChanged.connect(function () {
                                           attachmentsBusy.running = true
                                           vksdk.attachPhotoToMessage(imagePicker.selectedContent)
                                       })
                                   }
                    }

//                    BackgroundItem {
//                        width: parent.width / 2
//                        height: Theme.itemSizeSmall

//                        Row {
//                            anchors.centerIn: parent
//                            spacing: Theme.paddingSmall

//                            Image {
//                                width: Theme.iconSizeSmallPlus
//                                height: Theme.iconSizeSmallPlus
//                                source: "image://theme/icon-m-video"
//                            }

//                            Label {
//                                anchors.verticalCenter: parent.verticalCenter
//                                width: contentWidth
//                                font.pixelSize: Theme.fontSizeTiny
//                                text: qsTr("Video")
//                            }
//                        }
//                    }
               // }

              //  Row {
                //    width: parent.width
                  //  height: childrenRect.height

//                    BackgroundItem {
//                        width: parent.width / 2
//                        height: Theme.itemSizeSmall

//                        Row {
//                            anchors.centerIn: parent
//                            spacing: Theme.paddingSmall

//                            Image {
//                                width: Theme.iconSizeSmallPlus
//                                height: Theme.iconSizeSmallPlus
//                                source: "image://theme/icon-m-music"
//                            }

//                            Label {
//                                anchors.verticalCenter: parent.verticalCenter
//                                width: contentWidth
//                                font.pixelSize: Theme.fontSizeTiny
//                                text: qsTr("Audio")
//                            }
//                        }
//                    }

                    MenuItem {
                        width: parent.width
                        height: Theme.itemSizeMedium

                        Row {
                            //anchors.centerIn: parent
                            spacing: Theme.paddingMedium

                            Image {
                                width: Theme.iconSizeSmallPlus
                                height: Theme.iconSizeSmallPlus
                                source: "image://theme/icon-m-document"
                            }

                            Label {
                                anchors.verticalCenter: parent.verticalCenter
                                width: contentWidth
                                font.pixelSize: Theme.fontSizeTiny
                                text: qsTr("Document")
                            }
                        }

                        onClicked: if (!attachmentsBusy.running) {
                                       var imagePicker = pageStack.push("Sailfish.Pickers.ContentPickerPage")
                                       imagePicker.selectedContentChanged.connect(function () {
                                           attachmentsBusy.running = true
                                           vksdk.attachDocToMessage(imagePicker.selectedContent)
                                       })
                                   }
                    }
               // }

//                Row {
//                    width: parent.width
//                    height: childrenRect.height

//                    BackgroundItem {
//                        width: parent.width / 2
//                        height: Theme.itemSizeSmall

//                        Row {
//                            anchors.centerIn: parent
//                            spacing: Theme.paddingSmall

//                            Image {
//                                width: Theme.iconSizeSmallPlus
//                                height: Theme.iconSizeSmallPlus
//                                source: "image://theme/icon-m-toy"
//                            }

//                            Label {
//                                anchors.verticalCenter: parent.verticalCenter
//                                width: contentWidth
//                                font.pixelSize: Theme.fontSizeTiny
//                                text: qsTr("Gift")
//                            }
//                        }
//                   }

//                    BackgroundItem {
//                        width: parent.width / 2
//                        height: Theme.itemSizeSmall

//                        Row {
//                            anchors.centerIn: parent
//                            spacing: Theme.paddingSmall

//                            Image {
//                                width: Theme.iconSizeSmallPlus
//                                height: Theme.iconSizeSmallPlus
//                                source: "image://theme/icon-m-location"
//                            }

//                            Label {
//                                anchors.verticalCenter: parent.verticalCenter
//                                width: contentWidth
//                                font.pixelSize: Theme.fontSizeTiny
//                                text: qsTr("Map")
//                            }
//                        }
//                    }
//                }
            }
        }

        TouchInteractionHint {
            direction: TouchInteraction.Up
            Component.onCompleted: if (settings.attachmentsHintCounter() < 3) {
                                       settings.incrementAttachmentsHintCounter()
                                       start()
                                   }
        }
    }

    Timer {
        id: typingTimer
        interval: 10000
    }

    Connections {
        target: vksdk
        onSavedPhoto: {
            attachmentsList += name + ","
            attachmentsBusy.running = false;
        }
        onSavedDoc: {
            attachmentsList += name + ","
            attachmentsBusy.running = false;
        }
        onUserTyping: {
            if (chatId !== 0) { if (chatId === historyId) typingLabel.visible = true }
            else if (userId === historyId) typingLabel.visible = true
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) pageStack.pushAttached(Qt.resolvedUrl("AudioPlayerPage.qml"))
        else if (status === PageStatus.Deactivating) vksdk.messages.markAsRead(historyId)
    }

    Component.onCompleted: {
        vksdk.messagesModel.clear()
        vksdk.messages.getHistory(historyId)
    }
}

