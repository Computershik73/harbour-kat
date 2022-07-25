# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-kat

CONFIG += sailfishapp
greaterThan(QT_VERSION, 5.5) {
    CONFIG += C++11
} else {
    QMAKE_CXXFLAGS += -std=gnu++11
}

QT += multimedia core gui

include("vksdk/vksdk.pri")

HEADERS += \
    src/settingswrapper.h \
    src/mediaplayerwrapper.h \
    src/playlistmodel.h \
    src/filesaver.h \
    src/utils.h

SOURCES += \
    src/harbour-kat.cpp \
    src/settingswrapper.cpp \
    src/mediaplayerwrapper.cpp \
    src/playlistmodel.cpp \
    src/filesaver.cpp \
    src/utils.cpp

OTHER_FILES += \
    qml/harbour-kat.qml \
    qml/cover/*.qml \
    qml/pages/*.qml \
    qml/views/*.qml \
    translations/*.ts \
    rpm/harbour-kat.spec \
    rpm/harbour-kat.changes.in \
    LICENSE \
    README.md

CONFIG += sailfishapp_i18n
TRANSLATIONS += \
    translations/harbour-kat-ru.ts \
    translations/harbour-kat-it.ts \
    translations/harbour-kat-nl.ts \
    translations/harbour-kat-az.ts

DISTFILES += \
    qml/pages/MainMenuPage.qml \
    qml/pages/LoginPage.qml \
    qml/pages/DialogsListPage.qml \
    qml/pages/ProfilePage.qml \
    qml/pages/ImageViewPage.qml \
    qml/pages/FriendsListPage.qml \
    qml/views/UserListItem.qml \
    qml/pages/DialogPage.qml \
    qml/views/MessageItem.qml \
    qml/views/FwdMessages.qml \
    qml/views/AttachmentsView.qml \
    qml/views/GeoItem.qml \
    qml/views/PhotosView.qml \
    qml/views/VideosView.qml \
    qml/views/AudiosView.qml \
    qml/views/DocumentsView.qml \
    qml/views/NewsView.qml \
    qml/pages/WallPostPage.qml \
    qml/views/WallPostView.qml \
    qml/views/RepostView.qml \
    qml/pages/NewsfeedPage.qml \
    qml/views/DialogImage.qml \
    qml/pages/AudioPlayerPage.qml \
    qml/pages/VideoPlayerPage.qml \
    qml/pages/AboutPage.qml \
    qml/views/LinksView.qml \
    qml/views/MoreButton.qml \
    qml/views/CollapsedView.qml \
    qml/views/BoxBackground.qml \
    qml/pages/GroupsListPage.qml \
    qml/pages/GroupProfilePage.qml \
    qml/views/WallItem.qml \
    qml/pages/RepostPage.qml \
    qml/pages/StatisticsPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/PhotoAlbumPage.qml \
    qml/pages/TopicsPage.qml \
    qml/pages/TopicPage.qml

