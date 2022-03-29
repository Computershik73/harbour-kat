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

#include "longpoll.h"

LongPoll::LongPoll(QObject *parent) : QObject(parent) {
    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

LongPoll::~LongPoll() {
    delete _manager;
}

void LongPoll::setAccessToken(QString value) {
    _accessToken = value;
}

void LongPoll::getLongPollServer() {
    QUrl url("https://api.vk.com/method/messages.getLongPollServer");
    QUrlQuery query;
    query.addQueryItem("access_token", _accessToken);
    query.addQueryItem("v", "5.93");
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "com.vk.vkclient/12 (unknown, iPhone OS 9.3.5, iPhone, Scale/2.000000)");

    _manager->get(request);
}

void LongPoll::finished(QNetworkReply *reply) {
    QJsonDocument jDoc = QJsonDocument::fromJson(reply->readAll());
    if (_server.isNull() || _server.isEmpty()) {
        QJsonObject jObj = jDoc.object().value("response").toObject();
        _server = jObj.value("server").toString();
        _key = jObj.value("key").toString();
        _ts = jObj.value("ts").toInt();
        doLongPollRequest();
    } else {
        QJsonObject jObj = jDoc.object();
        if (jObj.contains("failed")) {
            if (jObj.value("failed").toInt() == 1) {
                _ts = jObj.value("ts").toInt();
                doLongPollRequest();
            } else {
                _server.clear();
                _key.clear();
                _ts = 0;
                getLongPollServer();
            }
        } else {
            _ts = jObj.value("ts").toInt();
            parseLongPollUpdates(jObj.value("updates").toArray());
            doLongPollRequest();
        }
    }
    reply->deleteLater();
}

void LongPoll::doLongPollRequest() {
    QUrl url("https://" + _server);
    QUrlQuery query;
    query.addQueryItem("act", "a_check");
    query.addQueryItem("key", _key);
    query.addQueryItem("ts", QString("%1").arg(_ts));
    query.addQueryItem("wait", "25");
    query.addQueryItem("mode", "10");
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "com.vk.vkclient/12 (unknown, iPhone OS 9.3.5, iPhone, Scale/2.000000)");


    _manager->get(request);
}

void LongPoll::parseLongPollUpdates(QJsonArray updates) {
    for (int index = 0; index < updates.size(); ++index) {
        QJsonArray update = updates.at(index).toArray();
        qDebug() << update;
        switch (update.at(0).toInt()) {
        case 4:
            emit gotNewMessage(update.at(1).toInt());
            break;
        case 6:
            emit readMessages(update.at(1).toInt(), update.at(2).toInt(), false);
            break;
        case 7:
            emit readMessages(update.at(1).toInt(), update.at(2).toInt(), true);
            break;
        case 8:
//            qDebug() << "--------------";
//            qDebug() << "User" << update.at(1).toInt() << "is online";
//            qDebug() << "--------------";
            break;
        case 9:
//            qDebug() << "--------------";
//            qDebug() << "User" << update.at(1).toInt() << "is offline";
//            qDebug() << "--------------";
            break;
        case 61:
            emit userTyping(update.at(1).toInt(), 0);
            break;
        case 62:
            emit userTyping(update.at(1).toInt(), update.at(2).toInt());
            break;

        case 80:
            emit unreadDialogsCounterUpdated(update.at(1).toInt());
            break;

        default:
            break;
        }
    }
}

