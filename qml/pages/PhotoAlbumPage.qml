import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: photoAlbumPage

    property var ownerId
    property int album: 0

    ComboBox {
        id: albumChooser
        anchors.top: parent.top
        width: parent.width
        label: qsTr("Album:")

        menu: ContextMenu {

            MenuItem {
                text: qsTr("All photos")
                onClicked: {
                    album = 0
                    vksdk.photosModel.clear()
                    vksdk.photos.getAll(ownerId)
                }
            }

            Repeater {
                id: albumsList
                model: ListModel {}
                delegate: MenuItem {
                    text: albumTitle
                    onClicked: {
                        album = albumId
                        vksdk.photosModel.clear()
                        vksdk.photos.get(ownerId, albumId)
                    }
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            z: -1
            gradient: Gradient {

                GradientStop {
                    position: 0.0
                    color: Theme.rgba(Theme.highlightBackgroundColor, 0.15)
                }

                GradientStop {
                    position: 1.0
                    color: "transparent"
                }
            }
        }
    }

    SilicaGridView {
        anchors.fill: parent
        anchors.topMargin: albumChooser.height
        clip: true
        model: vksdk.photosModel

        cellWidth: width / 3
        cellHeight: cellWidth

        delegate: Image {
            width: parent.width / 3
            height: width
            fillMode: Image.PreserveAspectCrop
            source: photoUrl

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("../pages/ImageViewPage.qml"), { current: index,
                                       fromAlbum: true })
                }
            }
        }

        PushUpMenu {
            visible: true

            MenuItem {
                text: qsTr("Load more")
                onClicked: {
                    if (album === 0) vksdk.photos.getAll(ownerId, vksdk.photosModel.size)
                    else vksdk.photos.get(ownerId, album, vksdk.photosModel.size)
                    //if (vksdk.photosModel.size !== vksdk.photosModel.count) {
                     //   visible = false
                    //}
                }
            }
        }

        VerticalScrollDecorator {}
    }

    Connections {
        target: vksdk
        onGotPhotoAlbums: {
            var id
            for (var index in data) {
                if (index % 2 === 0) id = data[index]
                else albumsList.model.append({ albumId: id, albumTitle: data[index] })
            }
        }
    }

    Component.onCompleted: {
        vksdk.photosModel.clear()
        vksdk.photos.getAlbums(ownerId)
        vksdk.photos.getAll(ownerId)
    }
}
