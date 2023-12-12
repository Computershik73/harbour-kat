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

#include "vksdk.h"

VkSDK::VkSDK(QObject *parent) : QObject(parent) {
    // basic:
    _api = new ApiRequest(this);
    _auth = new Authorization(this);
    qRegisterMetaType<Authorization*>("Authorization*");
    connect(_api, &ApiRequest::gotResponse, this, &VkSDK::gotResponse);
//    connect(_api, &ApiRequest::gotResponse,
//            [this] (QJsonValue value, ApiRequest::TaskType type) { QtConcurrent::run(this, &VkSDK::gotResponse, value, type); });
//            [this] (QJsonValue value, ApiRequest::TaskType type) { gotResponse(value, type); });

    _longPoll = new LongPoll(this);
    connect(_longPoll, SIGNAL(gotNewMessage(int)), this, SLOT(_gotNewMessage(int)));
    connect(_longPoll, SIGNAL(readMessages(qint64,qint64,bool)), this, SLOT(_readMessages(qint64,qint64,bool)));
    connect(_longPoll, SIGNAL(userTyping(qint64,qint64)), this, SLOT(_userTyping(qint64,qint64)));

    // requests:
    _account = new Account(this);
    _audios = new Audios(this);
    _board = new Board(this);
    _friends = new Friends(this);
    _groups = new Groups(this);
    _likes = new Likes(this);
    _messages = new Messages(this);
    _newsfeed = new Newsfeed(this);
    _photos = new Photos(this);
    _docs = new Docs(this);
    _stats = new Stats(this);
    _users = new Users(this);
    _videos = new Videos(this);
    _wall = new Wall(this);
   // _longPoll->setApi(_api);
    _account->setApi(_api);
    _audios->setApi(_api);
    _board->setApi(_api);
    _friends->setApi(_api);
    _groups->setApi(_api);
    _likes->setApi(_api);
    _messages->setApi(_api);
    _newsfeed->setApi(_api);
    _photos->setApi(_api);
    _docs->setApi(_api);
    _stats->setApi(_api);
    _users->setApi(_api);
    _videos->setApi(_api);
    _wall->setApi(_api);
    qRegisterMetaType<LongPoll*>("LongPoll*");
    qRegisterMetaType<Account*>("Account*");
    qRegisterMetaType<Audios*>("Audios*");
    qRegisterMetaType<Board*>("Board*");
    qRegisterMetaType<Friends*>("Friends*");
    qRegisterMetaType<Groups*>("Groups*");
    qRegisterMetaType<Likes*>("Likes*");
    qRegisterMetaType<Messages*>("Messages*");
    qRegisterMetaType<Newsfeed*>("Newsfeed*");
    qRegisterMetaType<Photos*>("Photos*");
    qRegisterMetaType<Docs*>("Docs*");
    qRegisterMetaType<Stats*>("Stats*");
    qRegisterMetaType<Users*>("Users*");
    qRegisterMetaType<Videos*>("Videos*");
    qRegisterMetaType<Wall*>("Wall*");

    // objects:
    qRegisterMetaType<Group*>("Group*");
    qRegisterMetaType<News*>("News*");
    qRegisterMetaType<User*>("User*");

    //models:
    _commentsModel = new CommentsModel(this);
    _dialogsListModel = new DialogsListModel(this);
    _friendsListModel = new FriendsListModel(this);
    _groupsListModel = new GroupsListModel(this);
    _messagesModel = new MessagesModel(this);
    _newsfeedModel = new NewsfeedModel(this);
    _wallModel = new NewsfeedModel(this);
    _photosModel = new PhotosModel(this);
    qRegisterMetaType<CommentsModel*>("CommentsModel*");
    qRegisterMetaType<DialogsListModel*>("DialogsListModel*");
    qRegisterMetaType<FriendsListModel*>("FriendsListModel*");
    qRegisterMetaType<GroupsListModel*>("GroupsListModel*");
    qRegisterMetaType<MessagesModel*>("MessagesModel*");
    qRegisterMetaType<NewsfeedModel*>("NewsfeedModel*");
    qRegisterMetaType<PhotosModel*>("PhotosModel*");


//    qRegisterMetaType<Audio*>("Audio*");
//    qRegisterMetaType<Document*>("Document*");
//    qRegisterMetaType<Photo*>("Photo*");
//    qRegisterMetaType<Friend*>("Friend*");
//    qRegisterMetaType<Video*>("Video*");
}

VkSDK::~VkSDK() {
    delete _api;
    delete _auth;
    delete _longPoll;

    delete _account;
    delete _audios;
    delete _board;
    delete _friends;
    delete _groups;
    delete _likes;
    delete _messages;
    delete _newsfeed;
    delete _photos;
    delete _docs;
    delete _stats;
    delete _users;
    delete _videos;
    delete _wall;

    delete _commentsModel;
    delete _dialogsListModel;
    delete _friendsListModel;
    delete _groupsListModel;
    delete _messagesModel;
    delete _newsfeedModel;
    delete _wallModel;
    delete _photosModel;

//    delete _selfProfile;
}

void VkSDK::setAccessTocken(QString value) {
    _api->setAccessToken(value);
    _longPoll->setAccessToken(value);
}

bool VkSDK::checkToken(QString token) {
    QUrl urll("https://api.vk.com/method/audio.get");
    QUrlQuery query;

    query.addQueryItem("count", "1");
    query.addQueryItem("access_token", token);
    query.addQueryItem("v", "5.91");
    urll.setQuery(query);
    QNetworkRequest request(urll);
    request.setRawHeader("User-Agent", "com.vk.vkclient/1654 (iPhone, iOS 12.2, iPhone8,4, Scale/2.0)");

    request.setRawHeader("Authorization", "Bearer "+token.toUtf8());
    qDebug () << request.rawHeader("Authorization");
    QNetworkAccessManager* _manager = new QNetworkAccessManager(this);
   // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    QNetworkReply *reply = _manager->get(request);
    QEventLoop looppp;
    QObject::connect(reply, SIGNAL(finished()) , &looppp, SLOT(quit()));
    looppp.exec();
     QByteArray dataaa = reply->readAll();
     QString DataAsString     = QString::fromUtf8(dataaa);
    // qDebug() << DataAsString;
     if (DataAsString.contains("error")) {
         return false;
     } else if (DataAsString.contains("authorization failed")) {
         return false;
     } else {
         return true;
     }
}

void VkSDK::setUserId(int value) {
    _userId = value;
}

Authorization *VkSDK::auth() const {
    return _auth;
}

LongPoll *VkSDK::longPoll() const {
    return _longPoll;
}

Account *VkSDK::account() const {
    return _account;
}

Audios *VkSDK::audios() const {
    return _audios;
}

Board *VkSDK::board() const {
    return _board;
}

Friends *VkSDK::friends() const {
    return _friends;
}

Groups *VkSDK::groups() const {
    return _groups;
}

Likes *VkSDK::likes() const {
    return _likes;
}

Messages *VkSDK::messages() const {
    return _messages;
}

Photos *VkSDK::photos() const {
    return _photos;
}

Docs *VkSDK::docs() const {
    return _docs;
}

Stats *VkSDK::stats() const {
    return _stats;
}

Newsfeed *VkSDK::newsfeed() const {
    return _newsfeed;
}

Wall *VkSDK::wall() const {
    return _wall;
}

CommentsModel *VkSDK::commentsModel() const {
    return _commentsModel;
}

Users *VkSDK::users() const {
    return _users;
}

Videos *VkSDK::videos() const {
    return _videos;
}

DialogsListModel *VkSDK::dialogsListModel() const {
    return _dialogsListModel;
}

FriendsListModel *VkSDK::friendsListModel() const {
    return _friendsListModel;
}

GroupsListModel *VkSDK::groupsListModel() const {
    return _groupsListModel;
}

MessagesModel *VkSDK::messagesModel() const {
    return _messagesModel;
}

NewsfeedModel* VkSDK::newsfeedModel() const {
    return _newsfeedModel;
}

bool VkSDK::isNewsfeedEmpty() {
    return _newsfeedModel->size()<=0;
}

NewsfeedModel *VkSDK::wallModel() const {
    return _wallModel;
}

PhotosModel *VkSDK::photosModel() const {
    return _photosModel;
}

void VkSDK::attachPhotoToMessage(QString path) {
    _pathToPhoto = path;
    _photos->getMessagesUploadServer();
}

void VkSDK::attachDocToMessage(QString path) {
    _pathToDoc = path;
    _docs->getMessagesUploadServer();
}

void VkSDK::gotResponse(const QJsonValue &value, ApiRequest::TaskType type) {
    switch (type) {
    case ApiRequest::ACCOUNT_BAN_USER:
        emit banSettingChanged(true);
        break;
    case ApiRequest::ACCOUNT_UNBAN_USER:
        emit banSettingChanged(false);
        break;
    case ApiRequest::AUDIO_GET:
    case ApiRequest::AUDIO_SEARCH:
        parseAudiosList(value.toObject().value("items").toArray());
        break;
    case ApiRequest::BOARD_GET_TOPICS:
        parseTopicsList(value.toObject().value("items").toArray());
        break;
    case ApiRequest::FRIENDS_GET:
        parseEntireFriendsList(value.toObject().value("items").toArray());
        break;
    case ApiRequest::FRIENDS_GET_MUTUAL:
    case ApiRequest::FRIENDS_GET_ONLINE:
        parseLimitedFriendsList(value.toArray());
        break;
    case ApiRequest::GROUPS_GET:
        parseGroupsList(value.toObject().value("items").toArray());
        break;
    case ApiRequest::GROUPS_GET_BY_ID:
        emit gotGroup(Group::fromJsonObject(value.toArray().at(0).toObject()));
        break;
    case ApiRequest::MESSAGES_GET_BY_ID:
        parseNewMessage(value.toObject().value("items").toArray().at(0).toObject());
        break;
    case ApiRequest::MESSAGES_GET_CHAT:
        parseChatsInfo(value.toArray());
        break;
    case ApiRequest::MESSAGES_GET_DIALOGS:
        parseDialogsInfo(value.toObject());
        break;
    case ApiRequest::MESSAGES_GET_HISTORY:
        parseMessages(value.toObject().value("items").toArray());
        break;
    case ApiRequest::PHOTOS_GET_ALBUMS:
        parsePhotoAlbums(value.toObject().value("items").toArray());
        break;
    case ApiRequest::PHOTOS_GET:
    case ApiRequest::PHOTOS_GET_ALL:
        parsePhotosList(value.toObject());
        break;
    case ApiRequest::PHOTOS_GET_MESSAGES_UPLOAD_SERVER:
        parseUploadServerData(value.toObject());
        break;
    case ApiRequest::DOCS_GET_MESSAGES_UPLOAD_SERVER:
        parseUploadServerDataDoc(value.toObject());
        break;
    case ApiRequest::PHOTOS_SAVE_MESSAGES_PHOTO:
        parseSavedPhotoData(value.toArray());
        break;
    case ApiRequest::DOCS_SAVE_MESSAGES_DOC:
        parseSavedDocData(value.toObject());
        break;
    case ApiRequest::PHOTOS_UPLOAD_TO_SERVER:
        parseUploadedPhotoData(value.toObject());
        break;
    case ApiRequest::DOCS_UPLOAD_TO_SERVER:
        parseUploadedDocData(value.toObject());
        break;
    case ApiRequest::STATS_GET:
        parseStatistics(value.toArray());
        break;
    case ApiRequest::NEWSFEED_GET:
        parseNewsfeed(value.toObject(), false);
        break;
    case ApiRequest::USERS_GET:
        if (_messagePreview.isEmpty()) emit gotProfile(parseUserProfile(value.toArray()));
        else {
            emit gotNewMessage(QString("%1 %2").arg(parseUserProfile(value.toArray())->getFirstName())
                                               .arg(parseUserProfile(value.toArray())->getLastName()),
                               _messagePreview);
            _messagePreview.clear();
        }
        break;
    case ApiRequest::USERS_GET_FRIENDS:
        parseFriendsInfo(value.toArray());
        break;
    case ApiRequest::VIDEO_GET:
        emit gotVideo(parseVideoInfo(value.toObject().value("items").toArray()));
        break;
    case ApiRequest::BOARD_CREATE_COMMENT:
    case ApiRequest::WALL_CREATE_COMMENT:
        emit commentCreated();
        break;
    case ApiRequest::WALL_GET:
        parseNewsfeed(value.toObject(), true);
        break;
    case ApiRequest::WALL_GET_BY_ID:
        emit gotWallpost(parseWallpost(value.toObject().value("items").toArray()));
        break;
    case ApiRequest::BOARD_GET_COMMENTS:
    case ApiRequest::WALL_GET_COMMENTS:
        parseComments(value.toObject());
        break;
    default:
        break;
    }
}

void VkSDK::_gotNewMessage(int id) {
    _messages->getById(id);
}

void VkSDK::_readMessages(qint64 peerId, qint64 localId, bool out) {
    _dialogsListModel->readMessages(peerId, localId, out);
    _messagesModel->readMessages(peerId, localId, out);
}

void VkSDK::_userTyping(qint64 userId, qint64 chatId) {
    emit userTyping(userId, chatId);
}

void VkSDK::parseAudiosList(QJsonArray array) {
    if (array.size() > 0) {
        QVariantList audios;
        for (int index = 0; index < array.size(); ++index) {
            audios.append(QVariant::fromValue(Audio::fromJsonObject(array.at(index).toObject())));
        }
        emit gotUserAudios(audios);
    }
}

void VkSDK::parseChatsInfo(QJsonArray array) {
    //_chatsIds.clear();
    for (int index = 0; index < array.size(); ++index) {
        Chat *chat = Chat::fromJsonObject(array.at(index).toObject());
        if (chat->users().size()>0) {
        foreach (QVariant user, chat->users()) _usersIds.append(user.toString());
        }

        //foreach (QVariant user, chat->users()) _usersIds.append(user.toString());

        _dialogsListModel->addChat(chat);
    }
    if (array.size()>0) {
    _usersIds.removeDuplicates();
    _users->getUsersByIds(_usersIds);
    }
}

void VkSDK::parseComments(QJsonObject object) {
    QJsonArray comments = object.value("items").toArray();
    QJsonArray profiles = object.value("profiles").toArray();
    QJsonArray groups = object.value("groups").toArray();
    for (int index = 0; index < comments.size(); ++index) {
        Comment *comment = Comment::fromJsonObject(comments.at(index).toObject());
        _commentsModel->addComment(comment);
    }
    for (int index = 0; index < profiles.size(); ++index) {
        User *profile = User::fromJsonObject(profiles.at(index).toObject());
        _commentsModel->addUser(profile);
    }
    for (int index = 0; index < groups.size(); ++index) {
        Group *group = Group::fromJsonObject(groups.at(index).toObject());
        _commentsModel->addGroup(group);
    }
}

void VkSDK::parseDialogsInfo(QJsonObject object) {
    if (object.contains("unread_count")) emit gotUnreadCounter(object.value("unread_count").toInt());
    parseFriendsInfo(object.value("profiles").toArray());
    parseGroupsInfo(object.value("groups").toArray());
    QJsonArray dialogs = object.value("items").toArray();
    for (int index = 0; index < dialogs.size(); ++index) {
        Dialog *dialog = Dialog::fromJsonObject(dialogs.at(index).toObject());
        if (dialog->isChat()) _chatsIds.append(QString::number(dialog->lastMessage()->chatId() - 2000000000));
        else _usersIds.append(QString::number(dialog->lastMessage()->peerId()));
        _dialogsListModel->add(dialog);
    }
   if (_chatsIds.empty()) {
       //if (!(_usersIds.empty())) {
        _usersIds.removeDuplicates();

        _users->getUsersByIds(_usersIds);


         //}
    } else _messages->getChat(_chatsIds);
}

void VkSDK::resetDialogsModel() {

   // _dialogsListModel->modelReset(QAbstractItemModel);
}

void VkSDK::parseEntireFriendsList(QJsonArray array) {
    for (int index = 0; index < array.size(); ++index) {
        Friend *profile = Friend::fromJsonObject(array.at(index).toObject(), false);
        _friendsListModel->add(profile);
    }
}

void VkSDK::parseFriendsInfo(QJsonArray array) {
    if (!_usersIds.empty()) {
        _usersIds.clear();
        for (int index = 0; index < array.size(); ++index) {
            Friend *profile = Friend::fromJsonObject(array.at(index).toObject(), false);
            _dialogsListModel->addProfile(profile);
        }
    } else if (!_chatUsersIds.empty()) {
        _chatUsersIds.clear();
        for (int index = 0; index < array.size(); ++index) {
            Friend *profile = Friend::fromJsonObject(array.at(index).toObject(), false);
            _messagesModel->addProfile(profile);
        }
    } else {
        for (int index = 0; index < array.size(); ++index) {
            Friend *profile = Friend::fromJsonObject(array.at(index).toObject(), false);
            _friendsListModel->add(profile);
        }
    }
}

void VkSDK::parseGroupsInfo(QJsonArray array) {

    if (array.size() == 0) return;
    foreach (QJsonValue value, array) {
        _dialogsListModel->addProfile(Friend::fromJsonObject(value.toObject(), true));
    }

}


void VkSDK::parseGroupsList(QJsonArray array) {
    if (array.size() == 0) return;
    foreach (QJsonValue value, array) {
        _groupsListModel->add(Group::fromJsonObject(value.toObject()));
    }
}

void VkSDK::parseLimitedFriendsList(QJsonArray array) {
    QStringList ids;
    for (int index = 0; index < array.size(); ++index) {
        ids.append(QString::number(array.at(index).toInt()));
    }
    _users->getUsersByIds(ids);
}

void VkSDK::parseMessages(QJsonArray array) {
    for (int index = 0; index < array.size(); ++index) {
        Message *message = Message::fromJsonObject(array.at(index).toObject());
        _chatUsersIds.append(QString::number(message->fromId()));
        foreach (QObject *object, message->fwdMessagesList()) {
            Message *fwd = qobject_cast<Message*>(object);
            _chatUsersIds.append(QString::number(fwd->fromId()));
        }
        _messagesModel->add(message);
    };
    _chatUsersIds.removeDuplicates();
    _users->getUsersByIds(_chatUsersIds);
}

void VkSDK::parseNewMessage(QJsonObject object) {
    Message *message = Message::fromJsonObject(object);
    /*if (message->chat()) {
        message->setFromId(message->fromId());
        message->setPeerId(message->peerId());
    } else {
        message->setFromId(message->fromId());
        message->setPeerId(message->peerId());
    }*/
    // Update dialogs
    _dialogsListModel->update(message);
    // Update chat
    _messagesModel->addToBegin(message);
    // Show notification
     _users->getUserProfile(message->fromId());
    if (message->out()) return;
    _messagePreview = (message->hasAttachments() ? "[ 📎 ] " : "") + message->body();
    //_users->getUserProfile(message->fromId());
   // qDebug() << "...finished...";
}

void VkSDK::parseNewsfeed(QJsonObject object, bool isWall) {
    QJsonArray posts = object.value("items").toArray();
    QJsonArray profiles = object.value("profiles").toArray();
    QJsonArray groups = object.value("groups").toArray();
    QString nextFrom = object.value("next_from").toString();
    for (int index = 0; index < posts.size(); ++index) {
        News *post = News::fromJsonObject(posts.at(index).toObject());
        if (isWall) _wallModel->addNews(post);
        else _newsfeedModel->addNews(post);
    }
    for (int index = 0; index < profiles.size(); ++index) {
        User *profile = User::fromJsonObject(profiles.at(index).toObject());
        if (isWall) _wallModel->addUser(profile);
        else _newsfeedModel->addUser(profile);
    }
    for (int index = 0; index < groups.size(); ++index) {
        Group *group = Group::fromJsonObject(groups.at(index).toObject());
        if (isWall) _wallModel->addGroup(group);
        else _newsfeedModel->addGroup(group);
    }
    if (isWall) _wallModel->setCount(object.value("count").toInt());
    else _newsfeedModel->setNextFrom(nextFrom);
}

void VkSDK::parsePhotoAlbums(QJsonArray array) {
    QList<QString> data;
    foreach (QJsonValue val, array) {
        data.append(QString::number(val.toObject().value("id").toInt()));
        data.append(val.toObject().value("title").toString());
    }
    emit gotPhotoAlbums(data);
}

void VkSDK::parsePhotosList(QJsonObject object) {
    int count = object.value("count").toInt();
    QJsonArray items = object.value("items").toArray();
    for (int index = 0; index < items.size(); ++index) {
        Photo *photo = Photo::fromJsonObject(items.at(index).toObject());
        _photosModel->addPhoto(photo);
    }
    _photosModel->setCount(count);
}

void VkSDK::parseSavedPhotoData(QJsonArray array) {
    QJsonObject photo = array.at(0).toObject();
    emit savedPhoto(QString("photo%1_%2").arg(QString::number(photo.value("owner_id").toInt()))
                    .arg(QString::number(photo.value("id").toInt())));
}

void VkSDK::parseSavedDocData(QJsonObject doc) {
    QJsonObject docdoc = doc.value("doc").toObject();
    emit savedDoc(QString("doc%1_%2").arg(QString::number(docdoc.value("owner_id").toInt()))
                    .arg(QString::number(docdoc.value("id").toInt())));
}

void VkSDK::parseStatistics(QJsonArray array) {
    QList<int> data;
    foreach (QJsonValue val, array) {
        QJsonObject jObj = val.toObject();
        data.append(jObj.value("views").toInt());
        data.append(jObj.value("visitors").toInt());
    }
    emit gotStats(data);
}

void VkSDK::parseTopicsList(QJsonArray array) {
    QList<int> ids;
    QStringList titles;
    QList<bool> closed;
    foreach (QJsonValue val, array) {
        ids << val.toObject().value("id").toInt();
        titles << val.toObject().value("title").toString();
        closed << (val.toObject().value("is_closed").toInt() == 1);
    }
    emit gotTopics(ids, titles, closed);
}

void VkSDK::parseUploadedPhotoData(QJsonObject object) {
    QString server = QString::number(object.value("server").toInt());
    QString photo = object.value("photo").toString();
    QString hash = object.value("hash").toString();
    _photos->saveMessagesPhoto(photo, server, hash);
}

void VkSDK::parseUploadedDocData(QJsonObject object) {
    QJsonDocument docc(object);
    //qDebug() << docc.toJson(QJsonDocument::Compact);
    QString doc = object.value("file").toString();
    _docs->saveMessagesDoc(doc);
}

void VkSDK::parseUploadServerData(QJsonObject object) {
    QString uploadUrl = object.value("upload_url").toString();
    QString albumId = QString::number(object.value("album_id").toInt());
    QString userId = QString::number(object.value("user_id").toInt());
    _photos->uploadPhotoToServer(uploadUrl, albumId, userId, _pathToPhoto);
}

void VkSDK::parseUploadServerDataDoc(QJsonObject object) {
    QString uploadUrl = object.value("upload_url").toString();
    qDebug() << uploadUrl;
    //QString albumId = QString::number(object.value("album_id").toInt());
    //QString userId = QString::number(object.value("user_id").toInt());
    _docs->uploadDocToServer(uploadUrl, _pathToDoc);
}

User *VkSDK::parseUserProfile(QJsonArray array) {
    return array.size() == 1 ? User::fromJsonObject(array.at(0).toObject()) : new User();
}

Video *VkSDK::parseVideoInfo(QJsonArray array) {
    return array.size() == 1 ? Video::fromJsonObject(array.at(0).toObject()) : new Video();
}

News *VkSDK::parseWallpost(QJsonArray array) {
    return array.size() == 1 ? News::fromJsonObject(array.at(0).toObject()) : new News();
}

//User *VkSDK::selfProfile() const {
//    return _selfProfile;
//}

//void VkSDK::gotFriendsList(QList<QObject *> friendsList) {
//    emit gotFriends(QVariant::fromValue(friendsList));
//}

//void VkSDK::gotMessagesList(QList<QObject *> messagesList) {
//    foreach (QObject *object, messagesList) {
//        Message *message = qobject_cast<Message*>(object);
//        _messagesModel->add(message);
//        _chatUsersIds.append(QString("%1").arg(message->fromId()));
//    }
//    _users->get(_chatUsersIds);
//}

//void VkSDK::gotMutualFriendsIds(QVariantList ids) {
//    QStringList sIds;
//    foreach (QVariant id, ids) sIds.append(id.toString());
//    _users->get(sIds);
//}

//void VkSDK::gotNewsfeed(QList<News *> items, QList<User *> profiles, QList<Group *> groups, QString nextFrom)
//{
//    foreach (News *item, items) _newsfeedModel->addNews(item);
//    foreach (User *user, profiles) _newsfeedModel->addUser(user);
//    foreach (Group *group, groups) _newsfeedModel->addGroup(group);
//    _newsfeedModel->setNextFrom(nextFrom);
//    //    emit newsfeedModelChanged();
//}

//void VkSDK::gotUnreadDialogsCounter(int value) {
//    emit gotUnreadCounter(value);
//}

//void VkSDK::gotUserProfile(User *user) {
//    if (user->id() == _userId) {
//        _selfProfile = user;
//        emit gotSelfProfile();
//    } else emit gotProfile(user);
//}

//void VkSDK::gotUsersList(QList<QObject *> usersList) {
//    if (_usersIds.size() > 0) {
//        _usersIds.clear();
//        foreach (QObject *object, usersList) {
//            Friend *user = qobject_cast<Friend*>(object);
//            _dialogsListModel->addProfile(user);
//        }
//    } else if (_chatUsersIds.size() > 0) {
//        _chatUsersIds.clear();
//        foreach (QObject *object, usersList) {
//            Friend *user = qobject_cast<Friend*>(object);
//            _messagesModel->addProfile(user);
//        }
//    } else emit gotFriends(QVariant::fromValue(usersList));
//}

//void VkSDK::gotVideoObject(Video *video)
//{
//    emit gotVideo(video);
//}

//void VkSDK::gotWallpostObject(News *wallpost)
//{
//    emit gotWallpost(wallpost);
//}

//void VkSDK::gotChatsList(QList<QObject *> chatsList) {
//    _chatsIds.clear();
//    foreach (QObject *object, chatsList) {
//        Chat *chat = qobject_cast<Chat*>(object);
//        foreach (QVariant user, chat->users()) {
//            if (!_usersIds.contains(user.toString())) _usersIds.append(user.toString());
//        }
//        _dialogsListModel->addChat(chat);
//    }
//    _users->get(_usersIds);
//}

//void VkSDK::gotDialogList(QList<Dialog *> dialogsList) {
//    foreach (Dialog *dialog, dialogsList) {
//        _dialogsListModel->add(dialog);
//        if (dialog->isChat()) _chatsIds.append(QString("%1").arg(dialog->lastMessage()->chatId()));
//        else _usersIds.append(QString("%1").arg(dialog->lastMessage()->userId()));
//    }
//    if (_chatsIds.empty()) _users->get(_usersIds);
//    else _messages->getChat(_chatsIds);
//}

//QStringList VkSDK::_getIdsFromMessages(QList<QObject *> messages) {
//    QStringList ids;
//    for (int index = 0; index < messages.size(); ++index) {
//        Message *msg = qobject_cast<Message*>(messages.at(index));
//        QString id = QString("%1").arg(msg->fromId());
//        if (!ids.contains(id) && id != "0") {
//            ids.append(id);
//        }
//        if (!msg->fwdMessagesList().isEmpty()) ids += _getIdsFromMessages(msg->fwdMessagesList());
//    }
//    return ids;
//}

