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

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include "apirequest.h"

const char TASK_TYPE_KEY[] = "taskType";

ApiRequest::ApiRequest(QObject *parent) : QObject(parent) {
    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

ApiRequest::~ApiRequest() {
    delete _manager;
}

void ApiRequest::makeApiGetRequest(const QString &method, const QUrlQuery &q, TaskType type) {
    QUrlQuery query = q;
    query.addQueryItem("access_token", _accessToken);
    query.addQueryItem("v", API_VERSION);
    QUrl url(API_URL + method);
    url.setQuery(query.query());
    qDebug() << "URL: " << url.toString();
    QNetworkReply *reply = _manager->get(QNetworkRequest(url));
    reply->setProperty(TASK_TYPE_KEY, type);
}

void ApiRequest::makePostRequest(const QUrl &u, const QUrlQuery &query, QHttpMultiPart *multipart, TaskType type) {
    QUrl url = u;
    if (!query.isEmpty()) url.setQuery(query.query());
    QNetworkReply *reply = _manager->post(QNetworkRequest(url), multipart);
    reply->setProperty(TASK_TYPE_KEY, type);
    multipart->setParent(reply);
}

void ApiRequest::setAccessToken(const QString &token) {
    _accessToken = token;
}

void ApiRequest::finished(QNetworkReply *reply) {
    const QVariant type = reply->property(TASK_TYPE_KEY);
    if (type.isValid()) {
        const TaskType taskType = type.value<TaskType>();
        QJsonDocument jDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jObj = jDoc.object();
        if (taskType == PHOTOS_UPLOAD_TO_SERVER) {
            emit gotResponse(jObj, taskType);
        } else if (jObj.contains("response")) {
            QJsonValue jVal = jObj.value("response");
            QString strFromObj = QJsonDocument(jObj).toJson(QJsonDocument::Compact).toStdString().c_str();
                qDebug() << "Reply: " << strFromObj << "\n";
            emit gotResponse(jVal, taskType);
        } else if (jObj.contains("error")) {
            qDebug() << "Error in API request!";
        }
    }
    reply->deleteLater();
}
