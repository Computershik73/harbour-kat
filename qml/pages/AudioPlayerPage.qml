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
import Sailfish.Media 1.0

Page {

    SilicaListView {
        id: playlist
        anchors.fill: parent
        anchors.bottomMargin: Qt.inputMethod.visible ? 0 : audioPlayer.height
        clip: true
        currentIndex: -1
        model: player.model
        header: SearchField {
            width: parent.width
            placeholderText: qsTr("Search audio...")
            onTextChanged: {
                if (text.length !== 0) {
                    busyIndicator.running = true
                    vksdk.audios.search(text)
                }
                else {
                    busyIndicator.running = true
                    vksdk.audios.get()
                }
            }
        }
        delegate: MediaListItem {
            width: parent.width
            height: childrenRect.height

            duration: _duration
            title: _title
            subtitle: _subtitle
            playing: _playing

            onClicked: {
                player.jumpTo(index)
                if (!player.isPlaying) {
                    player.play()
                }
            }
        }

        PullDownMenu {

            MenuItem {
                text: qsTr("My audios")
                onClicked: {
                    busyIndicator.running = true
                    vksdk.audios.get()
                }
            }
            MenuItem {
                text: qsTr("Recommendations")
                onClicked: {
                    busyIndicator.running = true
                    vksdk.audios.getRecommendations()

                }
            }
        }

        BusyIndicator {
           id: busyIndicator
           anchors.centerIn: parent
           size: BusyIndicatorSize.Large
           running: false
        }

        VerticalScrollDecorator {}
    }

    MediaPlayerControlsPanel {
        id: audioPlayer
        active: true
        open: true
        playing: player.isPlaying
        //author: player.author
        //title: player.title
        duration: player.duration
        position: player.position / 1000

        onPlayPauseClicked: {
            if (playing) player.pause()
            else player.play()
        }
        onPreviousClicked: if (player.currentIndex > 0) player.prev()
        onNextClicked: if (player.currentIndex < player.size-1) player.next()
        onSliderReleased: player.seekTo(value)
        onRepeatClicked: {
            player.repeat = !player.repeat
            repeat = player.repeat ? MediaPlayerControls.RepeatTrack :
                                     MediaPlayerControls.NoRepeat
        }
        onShuffleClicked: {
            player.shuffle = !player.shuffle
            shuffle = player.shuffle ? MediaPlayerControls.ShuffleTracks :
                                       MediaPlayerControls.NoShuffle
        }
        onAddToPlaylist: vksdk.audios.add(player.ownerId, player.audioId)

        onOpenChanged: open = true
    }

    Connections {
        target: player
        onMediaChanged: {
            player.model.setPlayingIndex(player.currentIndex);
            playlist.positionViewAtIndex(player.currentIndex, ListView.Center)
        }
    }

    Connections {
        target: vksdk
        onGotUserAudios: {
//            console.log(audios.length)
            busyIndicator.running = false
            player.setPlaylist(audios, -1)
        }
    }
}

