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

#include "audios.h"

Audios::Audios(QObject *parent) : RequestBase(parent)
{}

void Audios::add(qint64 ownerId, qint64 audioId) {
    QUrlQuery query;
    query.addQueryItem("owner_id", QString::number(ownerId));
    query.addQueryItem("audio_id", QString::number(audioId));
    _api->makeApiGetRequest("audio.add", query, ApiRequest::AUDIO_ADD);
}

void Audios::get(qint64 ownerId, int count) {
    QUrlQuery query;
    if (ownerId != 0) query.addQueryItem("owner_id", QString::number(ownerId));
    query.addQueryItem("count", QString::number(count));
    _api->makeApiGetRequest("audio.get", query, ApiRequest::AUDIO_GET);
}

void Audios::getRecommendations(int count) {
    QUrlQuery query;
    //if (ownerId != 0) query.addQueryItem("owner_id", QString::number(ownerId));
    query.addQueryItem("count", QString::number(count));
    _api->makeApiGetRequest("audio.getRecommendations", query, ApiRequest::AUDIO_GET);
}

void Audios::search(QString _query) {
    QUrlQuery query;
    query.addQueryItem("q", _query);
    query.addQueryItem("auto_complete", "1");
    query.addQueryItem("lyrics", "0");
    query.addQueryItem("performer_only", "0");
    query.addQueryItem("sort", "2");
    query.addQueryItem("count", "300");
    _api->makeApiGetRequest("audio.search", query, ApiRequest::AUDIO_SEARCH);
}
