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

#include "wall.h"

Wall::Wall(QObject *parent) : RequestBase(parent)
{}

void Wall::get(int ownerId, int offset) {
    QUrlQuery query;
    query.addQueryItem("owner_id", QString::number(ownerId));
    query.addQueryItem("count", "20");
    query.addQueryItem("extended", "1");
    query.addQueryItem("fields", "photo_50");
    if (offset != 0) query.addQueryItem("offset", QString::number(offset));
    _api->makeApiGetRequest("wall.get", query, ApiRequest::WALL_GET);
}

void Wall::getById(int ownerId, int id) {
    QUrlQuery query;
    query.addQueryItem("posts", QString("%1_%2").arg(ownerId).arg(id));
    _api->makeApiGetRequest("wall.getById", query, ApiRequest::WALL_GET_BY_ID);
}

void Wall::getComments(int ownerId, int postId, int offset) {
    QUrlQuery query;
    query.addQueryItem("owner_id", QString::number(ownerId));
    query.addQueryItem("post_id", QString::number(postId));
    query.addQueryItem("count", "100");
    query.addQueryItem("need_likes", "1");
    query.addQueryItem("sort", "asc");
    query.addQueryItem("extended", "1");
    query.addQueryItem("fields", "photo_50");
    if (offset != 0) query.addQueryItem("offset", QString::number(offset));
    _api->makeApiGetRequest("wall.getComments", query, ApiRequest::WALL_GET_COMMENTS);
}

void Wall::createComment(int ownerId, int postId, const QString &message) {
    QUrlQuery query;
    query.addQueryItem("owner_id", QString::number(ownerId));
    query.addQueryItem("post_id", QString::number(postId));
    query.addQueryItem("message", message);
    _api->makeApiGetRequest("wall.createComment", query, ApiRequest::WALL_CREATE_COMMENT);
}

void Wall::repost(const QString &type, int ownerId, int postId, const QString &message) {
    QUrlQuery query;
    query.addQueryItem("object", QString("%1%2_%3").arg(type).arg(ownerId).arg(postId));
    query.addQueryItem("message", message);
    _api->makeApiGetRequest("wall.repost", query, ApiRequest::WALL_REPOST);
}
