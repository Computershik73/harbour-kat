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
import Nemo.Notifications 1.0
import Amber.Mpris 1.0
import QtMultimedia 5.0
import Nemo.DBus 2.0


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

            if (vksdk.checkToken(settings.accessToken())) {
                vksdk.setAccessTocken(settings.accessToken())
                vksdk.setUserId(settings.userId())
                return Qt.createComponent(Qt.resolvedUrl("pages/MainMenuPage.qml"))
            } else {
                settings.removeAccessToken()
                settings.removeUserId()
                return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
            }
        } else {
            return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
        }
    }

    Notification {
        id: commonNotification
        appIcon: "ru.ilyavysotsky.aurorakat"
        category: "AuroraKat"
        appName: "AuroraKat"
        urgency: Notification.Critical
        //replacesId: 0
        remoteActions: [
            { "name":    "default",
                "service": "ru.ilyavysotsky.aurorakat",
                "path":    "ru/ilyavysotsky/aurorakat",
                "iface":   "ru.ilyavysotsky.aurorakat",
                "method":  "activateApp" }

        ]
        onClicked: application.activate()

    }

    MediaPlayer {
        id: rootPlayer
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
        canSeek: true
        desktopEntry: "AuroraKat"

        playbackStatus: player.isPlaying ? Mpris.Playing : Mpris.Paused

        //  onMetaDataChanged: {
        /*var metadata = mprisPlayer.metaData
                metadata[Mpris.metadataToString(Mpris.albumArtist)] = player.author
                metadata[Mpris.metadataToString(Mpris.Title)] = player.title
                //mprisPlayer.metadata = metadata
                mprisPlayer.setMetadata(metadata)*/
        // }

        //  onArtistChanged: {
        /*var metadata = mprisPlayer.metadata

            console.log(artist)

            metadata['xesam:artist'] = [artist] // List of strings

            mprisPlayer.metadata = metadata*/

        //  mprisPlayer.artist = artist
        //}

        // onSongChanged: {
        /* var metadata = mprisPlayer.metadata
            metaData.title = track.title*/
        /*var metadata = mprisPlayer.metadata

            console.log(song)

            metadata['xesam:title'] = song // String

            mprisPlayer.metadata = metadata*/

        //   console.log(mprisPlayer.song)

        //mprisPlayer.song = song
        //}

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
            //commonNotification.close()
            commonNotification.replacesId = 0
            commonNotification.summary = name
            commonNotification.previewSummary = name
            commonNotification.body = preview
            commonNotification.previewBody = preview
            commonNotification.appIcon = "ru.ilyavysotsky.aurorakat"
            commonNotification.appName = "AuroraKat"
            commonNotification.urgency = Notification.Critical
            commonNotification.category = "x-nemo.messaging.im"

            commonNotification.publish()
        }
    }

    Connections {
        target: vksdk.longPoll
        onUnreadDialogsCounterUpdated: {
            console.log("onUnreadDialogsCounterUpdated", value)
            //messagesCounter.text = value
        }
    }

    Connections {
        target: netcfgmgr
        onConfigurationChanged: {
            console.log("onConfigurationChanged")
            //vksdk.longPoll.getLongPollServer()
        }
    }

    DBusAdaptor {
        id: dbus

        service: "ru.ilyavysotsky.aurorakat"
        iface: "ru.ilyavysotsky.aurorakat"
        path: "/ru/ilyavysotsky/aurorakat"

        xml: '  <interface name="ru.ilyavysotsky.aurorakat">\n' +
             '  <method name="activateApp" />\n' +
             '  </interface>\n'

        function activateApp()
        {
            if ( !application.applicationActive ) {
                application.activate()
            }
        }
    }


    Connections {
        target: player
        onMediaChanged: {
            var metaData = mprisPlayer.metadata
           // console.log(metaData)
            //console.log(player.title)

            mprisPlayer.metaData.title = player.title
            mprisPlayer.metaData.contributingArtist = player.author

        }


    }




}
