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

#include "messagesmodel.h"

MessagesModel::MessagesModel(QObject *parent) : QAbstractListModel(parent)
{}

int MessagesModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return _messages.size();
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    Message *message = _messages.at(index.row());

    switch (role) {
    case AvatarRole:
        if (_profiles.contains(message->fromId())) return QVariant(_profiles[message->fromId()]->photo50());
        if (_profiles.contains(message->fromId())) return QVariant(_profiles[message->fromId()]->photo50());
        return QVariant();

    case IdRole:
        return QVariant(message->id());

    case FromIdRole:
        return QVariant(message->fromId());

    case DateRole:
        return QVariant(message->date());

    case IsOutRole:
        return QVariant(message->out());

    case IsReadRole:
        return QVariant(message->readState());

    case BodyRole:
        return QVariant(message->body());

    case PhotosRole:
        return message->photos();

    case VideosRole:
        return message->videos();

    case AudiosRole:
        return message->audios();

    case DocumentsRole:
        return message->documents();

    case LinksRole:
        return message->links();

    case NewsRole:
        return message->news();

    case GeoTileRole:
        return QVariant(message->geoTile());

    case GeoMapRole:
        return QVariant(message->geoMap());

    case FwdMessagesRole:
        return message->fwdMessages();

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MessagesModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[AvatarRole] = "avatar";
    roles[IdRole] = "id";
    roles[FromIdRole] = "fromId";
    roles[DateRole] = "datetime";
    roles[IsOutRole] = "out";
    roles[IsReadRole] = "read";
    roles[BodyRole] = "body";
    roles[PhotosRole] = "photosList";
    roles[VideosRole] = "videosList";
    roles[AudiosRole] = "audiosList";
    roles[DocumentsRole] = "documentsList";
    roles[LinksRole] = "linksList";
    roles[NewsRole] = "newsList";
    roles[GeoTileRole] = "geoTileUrl";
    roles[GeoMapRole] = "geoMapUrl";
    roles[FwdMessagesRole] = "fwdMessagesList";
    return roles;
}

void MessagesModel::clear() {
    beginRemoveRows(QModelIndex(), 0, _messages.size());
    _messages.clear();
    _profiles.clear();
    endRemoveRows();

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void MessagesModel::add(Message *message) {
    beginInsertRows(QModelIndex(), _messages.size(), _messages.size());
    _messages.append(message);
    endInsertRows();

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void MessagesModel::addToBegin(Message *message) {
    if (_messages.isEmpty()) return;
    if (message->chat() && _messages.at(0)->chatId() != message->chatId()) return;
    if (!message->chat() && _messages.at(0)->fromId() != message->fromId()) return;

    beginInsertRows(QModelIndex(), 0, 0);
    _messages.insert(0, message);
    endInsertRows();

    QModelIndex index = createIndex(0, _messages.size(), static_cast<void *>(0));
    emit dataChanged(index, index);
}

void MessagesModel::addProfile(Friend *profile) {
    if (_profiles.contains(profile->id())) return;
    _profiles[profile->id()] = profile;

    QModelIndex startIndex = createIndex(0, 0, static_cast<void *>(0));
    QModelIndex endIndex = createIndex(_messages.size(), 0, static_cast<void *>(0));
    emit dataChanged(startIndex, endIndex);
}

void MessagesModel::readMessages(qint64 peerId, qint64 localId, bool out) {
    if (_messages.isEmpty()) return;
    if (_messages.at(0)->chat() && _messages.at(0)->chatId() != peerId) return;
    if (!_messages.at(0)->chat() && _messages.at(0)->userId() != peerId) return;
    foreach (Message *message, _messages) {
        if (message->id() <= localId && message->out() == out) message->setReadState(true);
    }
    QModelIndex startIndex = createIndex(0, 0, static_cast<void *>(0));
    QModelIndex endIndex = createIndex(_messages.size(), 0, static_cast<void *>(0));
    emit dataChanged(startIndex, endIndex);
}

int MessagesModel::size() const {
    return _messages.size();
}

