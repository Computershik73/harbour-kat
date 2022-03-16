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

#include "chat.h"
#include <QJsonDocument>

Chat::Chat(QObject *parent) : QObject(parent)
{}

Chat::~Chat() {
    _users.clear();
}

Chat *Chat::fromJsonObject(QJsonObject object)
{
    /*QString strFromObj = QJsonDocument(object).toJson(QJsonDocument::Compact).toStdString().c_str();
     qDebug() << "Chat: " << strFromObj << "\n";
    Chat *chat = new Chat();
    QJsonObject conversation = object.value("conversation").toObject();
    QJsonObject peer = conversation.value("peer").toObject();

    chat->setId(peer.value("id").toInt());
    QJsonObject chat_settings = object.value("chat_settings").toObject();
    chat->setTitle(chat_settings.value("title").toString());
    if (chat_settings.contains("photo")) {
     QJsonObject chat_photos = chat_settings.value("photo").toObject();
    if (chat_photos.contains("photo_50")) chat->setPhoto(chat_photos.value("photo_50").toString());
    }
    //chat->setUsers(object.value("users").toArray().toVariantList());
    return chat;*/

    Chat *chat = new Chat();
        chat->setId(object.value("id").toInt());
        chat->setTitle(object.value("title").toString());
        if (object.contains("photo_50")) chat->setPhoto(object.value("photo_50").toString());
        if (object.value("users").toArray().contains(0)) {
        chat->setUsers(object.value("users").toArray().toVariantList());
        } else {
             QVariantList vl;
            chat->setUsers(vl);
        }
        return chat;
}

qint64 Chat::id() const
{
     return _id + 2000000000;
}

void Chat::setId(qint64 id)
{
    _id = id;
}

QString Chat::title() const
{
    return _title;
}

void Chat::setTitle(const QString &title)
{
    _title = title;
}

QString Chat::photo() const
{
    return _photo;
}

void Chat::setPhoto(const QString &photo)
{
    _photo = photo;
}

QVariantList Chat::users() const
{
    return _users;
}

void Chat::setUsers(const QVariantList &users)
{
    _users = users;
}

