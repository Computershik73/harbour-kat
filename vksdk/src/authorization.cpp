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

#include "authorization.h"

Authorization::Authorization(QObject *parent) : QObject(parent)
{}

Authorization::~Authorization()
{}

/**
 * @brief The function builds URL for authorization.
 * @return The URL for authorization.
 */
QString Authorization::authUrl() {
    QUrl url("https://oauth.vk.com/authorize");
    QUrlQuery query;
    query.addQueryItem("client_id", "4803503");
    query.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");
    query.addQueryItem("display", "mobile");
    query.addQueryItem("scope", "friends,photos,audio,video,docs,notes,pages,status,wall,groups,messages,notifications,offline,stats");
    query.addQueryItem("response_type", "token");
    query.addQueryItem("v", "5.93");
    query.addQueryItem("revoke", "1");
    url.setQuery(query);
    return url.toString();
}

/**
 * @brief The function checks is a current URL is final. If true, get access token and user id
 *        or error code and error message.
 * @param url - URL for checking.
 */
void Authorization::tryToGetAccessToken(QString url) {
    QUrlQuery query(QUrl(url).fragment());
    if (query.hasQueryItem("access_token")) {
        emit authorized(query.queryItemValue("access_token"), query.queryItemValue("user_id"));
    } else if (query.hasQueryItem("error")) {
        emit error(query.queryItemValue("error"), query.queryItemValue("error_description"));
    }
}
