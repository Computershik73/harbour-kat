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

#include "message.h"

Message::Message(QObject *parent) : QObject(parent)
{}

Message::~Message() {
    qDeleteAll(_audios);
    qDeleteAll(_documents);
    qDeleteAll(_links);
    qDeleteAll(_news);
    qDeleteAll(_photos);
    qDeleteAll(_videos);
    qDeleteAll(_fwdMessages);
    _audios.clear();
    _documents.clear();
    _links.clear();
    _news.clear();
    _photos.clear();
    _videos.clear();
    _fwdMessages.clear();
}

Message *Message::fromJsonObject(QJsonObject object) {
    Message *message = new Message();
    message->setId(object.value("id").toInt());
    if (object.contains("user_id")) {
        message->setChat(false);
        message->setUserId(object.value("user_id").toInt());
    }
    if (object.contains("chat_id")) {
        message->setChat(true);
        message->setChatId(object.value("chat_id").toInt());
    }
    if (object.contains("from_id")) {
        message->setChat(false);
        message->setUserId(object.value("from_id").toInt());
    }

    if (object.contains("from_id")) message->setFromId(object.value("from_id").toInt());
    if (object.contains("date")) message->setDate(object.value("date").toInt());
    if (object.contains("read_state")) message->setReadState(object.value("read_state").toInt() == 1);
    if (object.contains("out")) message->setOut(object.value("out").toInt() == 1);
    if (object.contains("text")) message->setBody(object.value("text").toString());
    if (object.contains("geo")) {
        QJsonObject geo = object.value("geo").toObject();
        QStringList coords = geo.value("coordinates").toString().split(" ");
        message->setGeoMap(coords.at(0).toDouble(), coords.at(1).toDouble());
        message->setGeoTile(coords.at(0).toDouble(), coords.at(1).toDouble());
    }
    if (object.contains("attachments")) {

        QJsonArray attachments = object.value("attachments").toArray();
        if (attachments.contains(0)) {
            message->setHasAttachments(true);
        }
        for (qint64 index = 0; index < attachments.size(); ++index) {
            QJsonObject attachment = attachments.at(index).toObject();
            if (attachment.value("type").toString() == "gift") {
                //
            } else if (attachment.value("type").toString() == "photo") {
                message->addPhoto(Photo::fromJsonObject(attachment.value("photo").toObject()));
            } else if (attachment.value("type").toString() == "video") {
                message->addVideo(Video::fromJsonObject(attachment.value("video").toObject()));
            } else if (attachment.value("type").toString() == "audio") {
                message->addAudio(Audio::fromJsonObject(attachment.value("audio").toObject()));
            } else if (attachment.value("type").toString() == "doc") {
                message->addDocument(Document::fromJsonObject(attachment.value("doc").toObject()));
            } else if (attachment.value("type").toString() == "wall") {
                message->addNews(News::fromJsonObject(attachment.value("wall").toObject()));
            } else if (attachment.value("type").toString() == "wall_reply") {
                //
            } else if (attachment.value("type").toString() == "sticker") {
                //
            } else if (attachment.value("type").toString() == "link") {
                message->addLink(Link::fromJsonObject(attachment.value("link").toObject()));
            } else if (attachment.value("type").toString() == "market") {
                //
            } else if (attachment.value("type").toString() == "market_album") {
                //
            }
        }
    }
    if (object.contains("fwd_messages")) {
        QJsonArray fwds = object.value("fwd_messages").toArray();
        for (qint64 index = 0; index < fwds.size(); ++index) {
            QJsonObject fwd = fwds.at(index).toObject();
            message->addFwdMessages(Message::fromJsonObject(fwd));
        }
    }
    return message;
}

qint64 Message::id() const
{
    return _id;
}

void Message::setId(qint64 id)
{
    _id = id;
}

qint64 Message::userId() const
{
    return _userId;
}

void Message::setUserId(qint64 userId)
{
    _userId = userId;
}

qint64 Message::chatId() const
{
    return _chatId + 2000000000;
}

void Message::setChatId(qint64 chatId)
{
    _chatId = chatId;
}

qint64 Message::fromId() const
{
    return _fromId;
}

void Message::setFromId(qint64 fromId)
{
    _fromId = fromId;
}

qint64 Message::date() const
{
    return _date;
}

void Message::setDate(qint64 date)
{
    _date = date;
}

bool Message::readState() const
{
    return _readState;
}

void Message::setReadState(bool readState)
{
    _readState = readState;
}

bool Message::out() const
{
    return _out;
}

void Message::setOut(bool out)
{
    _out = out;
}

QString Message::body() const
{
    return _body;
}

void Message::setBody(QString body)
{
    _body = body.replace(QRegExp("((?:https?|ftp)://\\S+)"), QString("<a href=\"\\1\">\\1</a>"))
                .replace("\n", "<br>");
}

QVariant Message::audios() const
{
    return QVariant::fromValue(_audios);
}

QList<QObject *> Message::audiosList() const
{
    return _audios;
}

void Message::addAudio(Audio *audio)
{
    _audios.append(audio);
}

QVariant Message::documents() const
{
    return QVariant::fromValue(_documents);
}

QList<QObject *> Message::documentsList() const
{
    return _documents;
}

void Message::addDocument(Document *document)
{
    _documents.append(document);
}

QVariant Message::links() const
{
    return QVariant::fromValue(_links);
}

QList<QObject *> Message::linksList() const
{
    return _links;
}

void Message::addLink(Link *link)
{
    _links.append(link);
}

QVariant Message::news() const
{
    return QVariant::fromValue(_news);
}

QList<QObject *> Message::newsList() const
{
    return _news;
}

void Message::addNews(News *news)
{
    _news.append(news);
}

QVariant Message::photos() const
{
    return QVariant::fromValue(_photos);
}

QList<QObject *> Message::photosList() const
{
    return _photos;
}

void Message::addPhoto(Photo *photo)
{
    _photos.append(photo);
}

QVariant Message::videos() const
{
    return QVariant::fromValue(_videos);
}

QList<QObject *> Message::videosList() const
{
    return _videos;
}

void Message::addVideo(Video *video)
{
    _videos.append(video);
}

QVariant Message::fwdMessages() const
{
    return QVariant::fromValue(_fwdMessages);
}

QList<QObject *> Message::fwdMessagesList() const {
    return _fwdMessages;
}

void Message::addFwdMessages(Message *message)
{
    _fwdMessages.append(message);
}

QString Message::geoTile() const
{
    return _geoTile;
}

void Message::setGeoTile(double lat, double lon)
{
    _geoTile = QString("http://www.mapquestapi.com/staticmap/v4/getmap?key=AfRC0WKf0YDWjtNKyWuB3LCpdHUQeME6&size=400,200&zoom=18&center=%1,%2").arg(lat).arg(lon);
}

QString Message::geoMap() const
{
    return _geoMap;
}

void Message::setGeoMap(double lat, double lon)
{
    _geoMap = QString("http://www.openstreetmap.org/?mlat=%1&mlon=%2&zoom=19").arg(lat).arg(lon);
}

bool Message::hasAttachments() const {
    return _hasAttachments;
}

void Message::setHasAttachments(bool hasAttachments)
{
    _hasAttachments = hasAttachments;
}

bool Message::chat() const
{
    return _chat;
}

void Message::setChat(bool chat)
{
    _chat = chat;
}

