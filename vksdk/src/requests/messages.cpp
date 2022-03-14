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

#include "messages.h"
#include <QTime>
#include "src/utils.h"
#include <QDebug>

Messages::Messages(QObject *parent) : RequestBase(parent)
{}

void Messages::getById(int id) {
    QUrlQuery query;
    query.addQueryItem("message_ids", QString::number(id));
    _api->makeApiGetRequest("messages.getById", query, ApiRequest::MESSAGES_GET_BY_ID);
}

void Messages::getChat(const QStringList &ids) {
    QUrlQuery query;
    if (ids.size() == 1) query.addQueryItem("chat_id", ids.at(0));
    else if (ids.size() > 1) query.addQueryItem("chat_ids", ids.join(","));
    _api->makeApiGetRequest("messages.getChat", query, ApiRequest::MESSAGES_GET_CHAT);
}

void Messages::getDialogs(int offset) {
    QUrlQuery query;
    query.addQueryItem("offset", QString::number(offset));
    _api->makeApiGetRequest("messages.getDialogs", query, ApiRequest::MESSAGES_GET_DIALOGS);
}

void Messages::getHistory(int peerId, int offset) {
    QUrlQuery query;
    query.addQueryItem("peer_id", QString::number(peerId));
    query.addQueryItem("offset", QString::number(offset));
    _api->makeApiGetRequest("messages.getHistory", query, ApiRequest::MESSAGES_GET_HISTORY);
}

void Messages::markAsRead(int peerId) {
    QUrlQuery query;
    query.addQueryItem("peer_id", QString::number(peerId));
    _api->makeApiGetRequest("messages.markAsRead", query, ApiRequest::MESSAGES_MARK_AS_READ);
}

void Messages::send(int peerId, const QString &text, const QString &attachmentsList) {
    QUrlQuery query;
    query.addQueryItem("peer_id", QString::number(peerId));
    if (!text.isEmpty()) query.addQueryItem("message", text);
    if (!attachmentsList.isEmpty()) query.addQueryItem("attachment", attachmentsList);
    query.addQueryItem("intent", "default");
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    int randomValue = Utils::randInt(1,1000);
    query.addQueryItem("random_id", QString::number(randomValue));
    _api->makeApiGetRequest("messages.send", query, ApiRequest::MESSAGES_SEND);
}

void Messages::setActivity(int peerId) {
    QUrlQuery query;
    query.addQueryItem("peer_id", QString::number(peerId));
    query.addQueryItem("type", "typing");
    _api->makeApiGetRequest("messages.setActivity", query, ApiRequest::MESSAGES_SET_ACTIVITY);
}

//void Messages::gotResponse(QJsonValue value, ApiRequest::TaskType type) {
//    switch (type) {
//    case ApiRequest::MESSAGES_GET_HISTORY: {
//        QJsonArray messages = value.toObject().value("items").toArray();
//        QList<QObject*> messagesList;
//        for (int index = 0; index < messages.size(); ++index) {
//            messagesList.append(Message::fromJsonObject(messages.at(index).toObject()));
//        }
//        emit gotMessagesList(messagesList);
//        break;
//    }

//    case ApiRequest::MESSAGES_GET_DIALOGS: {
//        QJsonObject object = value.toObject();
//        if (object.contains("unread_dialogs")) emit gotUnreadDialogsCounter(object.value("unread_dialogs").toInt());
//        QJsonArray dialogs = value.toObject().value("items").toArray();
//        QList<Dialog*> dialogsList;
//        for (int index = 0; index < dialogs.size(); ++index) {
//            dialogsList.append(Dialog::fromJsonObject(dialogs.at(index).toObject()));
//        }
//        emit gotDialogsList(dialogsList);
//        break;
//    }

//    case ApiRequest::MESSAGES_GET_CHAT: {
//        QJsonArray chats = value.toArray();
//        QList<QObject*> chatsList;
//        for (int index = 0; index < chats.size(); ++index) {
//            chatsList.append(Chat::fromJsonObject(chats.at(index).toObject()));
//        }
//        emit gotChatsList(chatsList);
//        break;
//    }

//    default:
//        break;
//    }
//}

