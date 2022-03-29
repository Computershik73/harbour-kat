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

#include "audio.h"

Audio::Audio(QObject *parent) : QObject(parent)
{}

Audio *Audio::fromJsonObject(QJsonObject object) {
    Audio *audio = new Audio();
    if (object.contains("id")) audio->setId(object.value("id").toInt());
    if (object.contains("owner_id")) audio->setOwnerId(object.value("owner_id").toInt());
    if (object.contains("duration")) audio->setDuration(object.value("duration").toInt());
    if (object.contains("artist")) audio->setArtist(object.value("artist").toString());
    if (object.contains("title")) audio->setTitle(object.value("title").toString());
    if (object.contains("url")) audio->setUrl(object.value("url").toString());
    if (object.contains("link_mp3")) {
        audio->setUrl(object.value("link_mp3").toString());
        audio->setTitle(QString("Голосовое сообщение"));
        audio->setArtist(QString("Голосовое сообщение"));
    }

    return audio;
}

int Audio::id() const
{
    return _id;
}

void Audio::setId(int id)
{
    _id = id;
}

int Audio::ownerId() const
{
    return _ownerId;
}

void Audio::setOwnerId(int ownerId)
{
    _ownerId = ownerId;
}

int Audio::duration() const
{
    return _duration;
}

void Audio::setDuration(int duration)
{
    _duration = duration;
}

QString Audio::artist() const
{
    return _artist;
}

void Audio::setArtist(const QString &artist)
{
    _artist = artist;
}

QString Audio::title() const
{
    return _title;
}

void Audio::setTitle(const QString &title)
{
    _title = title;
}

QString Audio::url() const
{
    return _url;
}

void Audio::setUrl(const QString &url)
{
    _url = url;
}

