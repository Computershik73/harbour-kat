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
import org.nemomobile.notifications 1.0
import org.nemomobile.mpris 1.0
import QtMultimedia 5.0



ApplicationWindow
{
    id: application
     //property alias mprisPlayer: mprisPlayer

    function convertUnixtimeToString(unixtime) {
        var d = new Date(unixtime * 1000)
        var month = d.getMonth() + 1
        var minutes = d.getMinutes() < 10 ? "0" + d.getMinutes() : d.getMinutes()
        return d.getDate() + "." + month + "." + d.getFullYear() + " " + d.getHours() + ":" + minutes
    }



    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    initialPage: {
        if (settings.accessToken()) {
            vksdk.setAccessTocken(settings.accessToken())
            vksdk.setUserId(settings.userId())
            if (vksdk.checkToken(settings.accessToken())) {
            return Qt.createComponent(Qt.resolvedUrl("pages/MainMenuPage.qml"))
            } else {
            return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
            }
        } else {
            return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
        }
    }

    Notification {
        id: commonNotification
        category: "harbour-kat"
        remoteActions: [
            { "name":    "default",
              "service": "nothing",
              "path":    "nothing",
              "iface":   "nothing",
              "method":  "nothing" }
        ]
    }

    MprisPlayer {
            id: mprisPlayer
            property string artist
            property string song

           // property string artist: "Loading"
          //  property string song: "tags..."
          //  artist: "test"
          //  song: "testt"

            serviceName: "kat-music"
            identity: "Kat Music"
            supportedUriSchemes: ["file", "http"]
            supportedMimeTypes: ["audio/x-wav", "audio/x-vorbis+ogg", "audio/mpeg"]

            canControl: true

            canGoNext: true
            canGoPrevious: true
            canPause: true
            canPlay: true
            canSeek: false


            playbackStatus: player.isPlaying ? Mpris.Playing : Mpris.Paused

          //  onMetaDataChanged: {
                /*var metadata = mprisPlayer.metaData
                metadata[Mpris.metadataToString(Mpris.albumArtist)] = player.author
                metadata[Mpris.metadataToString(Mpris.Title)] = player.title
                //mprisPlayer.metadata = metadata
                mprisPlayer.setMetadata(metadata)*/
           // }

            onArtistChanged: {
                        var metadata = mprisPlayer.metadata

                        metadata[Mpris.metadataToString(Mpris.Artist)] = [artist] // List of strings

                        mprisPlayer.metadata = metadata
                    }

                    onSongChanged: {
                        var metadata = mprisPlayer.metadata

                        metadata[Mpris.metadataToString(Mpris.Title)] = song // String

                        mprisPlayer.metadata = metadata
                    }

            onPauseRequested: {
                    player.pause()
                }
                onPlayRequested: {
                    player.play()
                }
                onPlayPauseRequested: {
                    if (player.isPlaying) {
                    player.pause()
                    } else {
                    player.play()
                    }
                }
                onStopRequested: {
                    player.stop()
                }

                onNextRequested: {
                    player.next()
                }
                onPreviousRequested: {
                    player.prev()
                }
        }



    Connections {
        target: vksdk
        onGotNewMessage: {
            commonNotification.summary = name
            commonNotification.previewSummary = name
            commonNotification.body = preview
            commonNotification.previewBody = preview
            commonNotification.close()
            commonNotification.publish()
        }
    }

    Connections {
        target: vksdk.longPoll
        onUnreadDialogsCounterUpdated: {
            console.log("onUnreadDialogsCounterUpdated", value)
            messagesCounter.text = value
        }
    }

    Connections {
        target: player
        onMediaChanged: {
            //qDebug() << "mediachanged"

            mprisPlayer.song = player.title
            mprisPlayer.artist = player.author
            //var metaData = mprisPlayer.metaData
            //metaData['mpris:title'] = "test"
            //metaData['mpris:albumArtist'] = "test"
            //metaData[Mpris.metadataToString(Mpris.albumArtist)] = "test"
            //metaData[Mpris.metadataToString(Mpris.Title)] = "test"
           //mprisPlayer.metaData = metaData
            //metaData['xesam:title'] = "test"
            // metaData['xesam:albumArtist'] = "test"
             //mprisPlayer.setMetadata(metaData)

        }

       /* onStateChanged: {
            // qDebug() << "statechanged"
            var metaData = mprisPlayer.metaData
            // metaData['mpris:title'] = "app.streamMetaText1"
            // metaData['mpris:albumArtist'] = "app.stationName"
            //metadata[Mpris.metadataToString(Mpris.albumArtist)] = player.author
            //metadata[Mpris.metadataToString(Mpris.Title)] = player.title
            metaData[Mpris.metadataToString(Mpris.albumArtist)] = "test"
            metaData[Mpris.metadataToString(Mpris.Title)] = "test"
             mprisPlayer.setMetadata(metaData)
        }*/
    }




}
