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
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

Authorization::Authorization(QObject *parent) : QObject(parent)
{
    codeisrequired=false;
    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

Authorization::~Authorization()
{}

/**
 * @brief The function builds URL for authorization.
 * @return The URL for authorization.
 */
QString Authorization::authUrl() {
    QUrl url("https://oauth.vk.com/authorize");
    QUrlQuery query;
    query.addQueryItem("client_id", "3140623");
                       //"4803503");
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
void Authorization::tryToGetAccessToken(QString namepass, QString code) {
    QStringList w=namepass.split(" ", QString::SkipEmptyParts);
    QUrl urll("https://oauth.vk.com/token");
    QUrlQuery query;
//https://oauth.vk .com/token?grant_type=password&client_id=6146827&client_secret=qVxWRF1CwHERuIrKBnqe&username=имя&password=пароль&v=5.131&2fa_supported=1
    query.addQueryItem("grant_type", "password");
    query.addQueryItem("client_id", "6146827");
                       //"3140623");
                       //"4803503");

    query.addQueryItem("client_secret", "qVxWRF1CwHERuIrKBnqe");
    //query.addQueryItem("client_secret", "VeWdmVclDCtn6ihuP1nt");
    query.addQueryItem("username", w.at(0));
    query.addQueryItem("password", w.at(1));
    query.addQueryItem("2fa_supported", "1");
    query.addQueryItem("force_sms", "1");
    query.addQueryItem("device_id", "8DB2CDDB-0D89-4262-A91F-3A945DD63BF5");
    query.addQueryItem("external_device_id", "8DB2CDDB-0D89-4262-A91F-3A945DD63BF5");

    if (code!="") {
       query.addQueryItem("code", code);
    }
    query.addQueryItem("scope", "notify,friends,photos,audio,video,docs,notes,pages,status,offers,questions,wall,groups,messages,notifications,stats,ads,offline");
    query.addQueryItem("v", "5.153");
    urll.setQuery(query);
    QNetworkRequest request(urll);
    request.setRawHeader("User-Agent", "com.vk.vkclient/1654 (iPhone, iOS 12.2, iPhone8,4, Scale/2.0)");


    QNetworkReply *reply = _manager->get(request);
    char TASK_TYPE_K[] = "taskType";
    reply->setProperty(TASK_TYPE_K, 0);



}

void Authorization::finished(QNetworkReply *reply) {

        QJsonDocument jDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jObj = jDoc.object();
        QString strFromObj = QJsonDocument(jObj).toJson(QJsonDocument::Compact).toStdString().c_str();
        qDebug() << strFromObj;
         if (jObj.contains("access_token")) {


            emit authorized(jObj.value("access_token").toString(), jObj.value("user_id").toInt());
        } else {
             if (strFromObj.contains("2fa")) {

                     QString validation_sid = jObj.value("validation_sid").toString();
                     QUrl urll("https://api.vk.com/method/auth.validatePhone");
                     QUrlQuery query;

                     query.addQueryItem("sid", validation_sid);
                     query.addQueryItem("v", "5.131");
                     urll.setQuery(query);
                     QNetworkRequest request(urll);
                     request.setRawHeader("User-Agent", "com.vk.vkclient/12 (unknown, iPhone OS 9.3.5, iPhone, Scale/2.000000)");

                     QNetworkReply *reply = _manager->get(request);
                     char TASK_TYPE_K[] = "taskType";
                     reply->setProperty(TASK_TYPE_K, 0);



                emit coderequired();


        } else {
                 if (strFromObj.contains("invalid")) {
                 qDebug() << "VK said: " << strFromObj;
                emit error(QString("Капча или неверный пароль"),QString("Капча или неверный пароль"));
                 }
             }
    }
    reply->deleteLater();
}


