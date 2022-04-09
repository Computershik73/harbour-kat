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

CoverBackground {
    id: cover

    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingMedium

        Row {
            spacing: Theme.paddingMedium
            anchors.horizontalCenter: parent.horizontalCenter
            Image {
                anchors.verticalCenter: messagesCounter.verticalCenter
                source: "image://theme/icon-m-message"
            }

            Label {
                id: messagesCounter
                text: "0"
                font.bold: true
                font.pixelSize: Theme.fontSizeHuge
            }
        }








                Label {
                    visible: player.isPlaying || player.isPaused
                    id: title
                    width: contentWidth>cover.width ? cover.width : contentWidth
                    //elide: Text.ElideRight
                    //wrapMode: Text.Wrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: player.isPlaying ? player.title : ""
                    font.pixelSize: Theme.fontSizeSmall
                    truncationMode: TruncationMode.Fade
                }
                Label {
                    visible: player.isPlaying || player.isPaused
                    id: artist
                    width: contentWidth>cover.width ? cover.width : contentWidth
                   // wrapMode: Text.Wrap
                   // elide: Text.ElideRight
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: player.isPlaying ? player.author : ""
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                    truncationMode: TruncationMode.Fade
                }


    }

//    CoverActionList {
//        id: coverAction

//        CoverAction {
//            iconSource: "image://theme/icon-cover-new"
//        }
//    }

    CoverActionList {
        id: activecover
            enabled: player.isPlaying || player.isPaused
            CoverAction {
                iconSource: "image://theme/icon-cover-previous-song"
                onTriggered: {
                    if (player.currentIndex > 0) {
                        player.prev()
                    if (!player.isPlaying) {
                        player.play()
                    }
                    }
                }
            }

            CoverAction {
                iconSource: player.isPlaying ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
                onTriggered: {
                        if (player.isPlaying) {
                            player.pause()

                        } else {
                            player.play()
                        }
                }
            }

            CoverAction {
                iconSource: "image://theme/icon-cover-next-song"
                onTriggered: {

                    if (player.currentIndex < player.size-1) {
                        player.next()
                    if (!player.isPlaying) {
                        player.play()
                    }
                 }
                }
            }


        }



    Connections {
        target: vksdk
        onGotUnreadCounter: {
            console.log("onGotUnreadCounter", value)
            messagesCounter.text = value
        }
    }

    Connections {
        target: vksdk.longPoll
        onUnreadDialogsCounterUpdated: {
            console.log("onUnreadDialogsCounterUpdated", value)
            messagesCounter.text = value
        }
    }
}


