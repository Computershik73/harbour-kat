/*
  Copyright (C) 2016-2017 Petr Vytovtov
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
    id: imageViewPage

    property var ownerIds
    property var photoIds
    property var imagesModel
    property var current
    property bool fromAlbum: false

    Drawer {
        id: drawer
        anchors.fill: parent

        open: true

        background: SilicaListView {
            anchors.fill: parent

            model: ListModel {

                ListElement { title: "Save" }
                ListElement { title: "Like" }
                ListElement { title: "Share" }
                ListElement { title: "Copy link" }
            }

            header: PageHeader {
                title: (slideshowView.currentIndex + 1) + ' / ' + (fromAlbum ? vksdk.photosModel.count : imagesModel.length)
            }

            delegate: ListItem {

                width: parent.width
                height: Theme.itemSizeSmall

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.title
                }

                onClicked: switch (index) {
                           case 0:
                               fileSaver.save(fromAlbum ? vksdk.photosModel.getUrl(slideshowView.currentIndex) : imagesModel[slideshowView.currentIndex])
                               break;
                           case 1:
                               if (fromAlbum)
                                   vksdk.likes.addPhoto(vksdk.photosModel.getOwnerId(slideshowView.currentIndex),
                                                        vksdk.photosModel.getPhotoId(slideshowView.currentIndex))
                               else
                                   vksdk.likes.addPhoto(ownerIds[slideshowView.currentIndex],
                                                        photoIds[slideshowView.currentIndex])
                               break;
                           case 2:
                               if (fromAlbum)
                                   pageStack.push(Qt.resolvedUrl("RepostPage.qml"),
                                                  { sourceId: vksdk.photosModel.getOwnerId(slideshowView.currentIndex),
                                                    postId: vksdk.photosModel.getPhotoId(slideshowView.currentIndex),
                                                    type: "photo" })
                               else
                                   pageStack.push(Qt.resolvedUrl("RepostPage.qml"), { sourceId: ownerIds[slideshowView.currentIndex],
                                                                                  postId: photoIds[slideshowView.currentIndex],
                                                                                  type: "photo" })
                               break;
                           case 3:
                               Clipboard.text = imagesModel[slideshowView.currentIndex]
                               break;
                           }
            }
        }

        SlideshowView {
            id: slideshowView
            width: imageViewPage.width
            height: imageViewPage.height
            model: fromAlbum ? vksdk.photosModel : imagesModel.length
            anchors.fill: parent
            delegate: Image {
                    id: imageView
                    width: imageViewPage.width
                    height: width * (sourceSize.height / sourceSize.width)
                    fillMode: Image.PreserveAspectFit
                    source: fromAlbum ? photoUrl : imagesModel[index]

                    PinchArea {
                        anchors.fill: parent
                        pinch.target: parent
                        pinch.minimumScale: 1
                        pinch.maximumScale: 4

                        MouseArea {
                            anchors.fill: parent
                            drag.target: imageView
                            drag.axis: Drag.XAndYAxis
                            drag.minimumX: (slideshowView.width - imageView.width * imageView.scale) / 2
                            drag.minimumY: (slideshowView.height - imageView.height * imageView.scale) / 2
                            drag.maximumX: Math.abs(slideshowView.width - imageView.width * imageView.scale) / 2
                            drag.maximumY: Math.abs(slideshowView.height - imageView.height * imageView.scale) / 2
                            onClicked: drawer.open = !drawer.open
                            onPositionChanged: { // TODO
                                console.log(drag.minimumX + " <= " + imageView.x + " <= " + drag.maximumX)
                                console.log(drag.minimumY + " <= " + imageView.y + " <= " + drag.maximumY)
                            }
                        }
                    }
                }
        }
    }

    Component.onCompleted: slideshowView.currentIndex = current
}

