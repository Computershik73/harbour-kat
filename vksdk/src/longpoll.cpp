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
#include <QEventLoop>
#include <QTimer>
#include "./requests/apirequest.h"

LongPoll::LongPoll(QObject *parent) : QObject(parent) {

    _netcfg_manager = new QNetworkConfigurationManager();
    _netcfg_manager->updateConfigurations();
    _net_name = _netcfg_manager->defaultConfiguration().name();

    _manager = new QNetworkAccessManager(this);

    connect(_netcfg_manager, SIGNAL(configurationChanged(const QNetworkConfiguration&)), this, SLOT(configurationChanged(const QNetworkConfiguration&)));
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

LongPoll::~LongPoll() {
    delete _manager;
}

void LongPoll::setAccessToken(QString value) {
    _accessToken = value;
}

inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void LongPoll::getLongPollServer() {
    QUrl url("https://api.vk.com/method/messages.getLongPollServer");
    QUrlQuery query;
    query.addQueryItem("access_token", _accessToken);
    query.addQueryItem("v", "5.93");
    query.addQueryItem("need_pts", "1");
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "com.vk.vkclient/1654 (iPhone, iOS 12.2, iPhone8,4, Scale/2.0)");

    _manager->get(request);
}

void LongPoll::configurationChanged(const QNetworkConfiguration&) {

    qDebug() << "netchanged" << _netcfg_manager->defaultConfiguration().name() << " " << _net_name;
  /*  if (_netcfg_manager->defaultConfiguration().name() != _net_name) {
        qDebug() << "netchanged" << _netcfg_manager->defaultConfiguration().name() << " " << _net_name;
        _net_name = _netcfg_manager->defaultConfiguration().name();
       //  setOffline();
        // setOffline(false);
    }*/
    //if ((_netcfg_manager->defaultConfiguration().name() == "Wired") && (_net_name != "Wired")) {
    //qDebug() << "\n abort \n";
      /*  foreach (auto &reply, _manager->findChildren<QNetworkReply *>()) {
            reply->abort();
            qDebug() << " aborted ";
        }*/
    //}


    //if (_netcfg_manager->defaultConfiguration().name() != _net_name) {
     //   _net_name = _netcfg_manager->defaultConfiguration().name();
       // if ((_netcfg_manager->defaultConfiguration().name() != "Wired")) {
         /*      delay(200);
                qDebug() << " trying to enable network ";
                _manager = new QNetworkAccessManager(this);
                connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

                _manager->setNetworkAccessible(QNetworkAccessManager::Accessible);
                getLongPollServer();*/
               // getLongPollHistoryRequest();
        //}
    //}

    //if ((_netcfg_manager->defaultConfiguration().name() != "Wired") && (_net_name == "Wired")) {
        //delete _manager;
      /*  qDebug() << "netchanged" << _netcfg_manager->defaultConfiguration().name() << " " << _net_name;

        _manager = new QNetworkAccessManager(this);
        connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));


        delay(2000);
        getLongPollHistoryRequest();*/
       // setOffline();
       // setOffline(false);

    //}
}



void LongPoll::finished(QNetworkReply *reply) {
    if (reply->error()!= QNetworkReply::NoError) {

        qDebug() << "\n err " << reply->errorString();
       //return;
        // reply->abort();
        qDebug() << " err2 ";
        //delete _manager;
        qDebug() << " err3 ";
       // _manager = new QNetworkAccessManager(this);
        //connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

        qDebug() << " err4 ";
        //delay(2000);

        //getLongPollHistoryRequest();
     //   setOffline();
        delay(200);
         qDebug() << " trying to enable network ";
         _manager = new QNetworkAccessManager(this);
         connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

         _manager->setNetworkAccessible(QNetworkAccessManager::Accessible);
         getLongPollServer();
        // getLongPollHistoryRequest();

        qDebug() << " err5 ";
      // setOffline(false);
       qDebug() << " err6 ";
        return;
    }
    QByteArray qb = reply->readAll();

    if (qb.isEmpty() || qb.isNull()) { return; }

    QJsonDocument jDoc = QJsonDocument::fromJson(qb);
    if (reply->request().url().query().contains("need_pts") && (jDoc.object().contains("response"))) {
        QJsonObject jObj = jDoc.object().value("response").toObject();
        _server = jObj.value("server").toString();
        _key = jObj.value("key").toString();
        _ts = jObj.value("ts").toInt();

            doLongPollRequest();

        reply->deleteLater();
        return;
    }
  /*  if ((_server.isNull() || _server.isEmpty()) && (jDoc.object().contains("response"))) {
        QJsonObject jObj = jDoc.object().value("response").toObject();
        _server = jObj.value("server").toString();
        _key = jObj.value("key").toString();
        _ts = jObj.value("ts").toInt();
        doLongPollRequest();
    } else {*/
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
            if (jObj.contains("history") && (jObj.value("history").toArray().size() > 0)) {
                parseLongPollUpdates(jObj.value("history").toArray());
                doLongPollRequest();
            }
            if (jObj.contains("updates") && (jObj.value("updates").toArray().size() > 0)) {
                parseLongPollUpdates(jObj.value("updates").toArray());
            }
            if (jObj.contains("pts")) {
                _pts = jObj.value("pts").toInt();
                //doLongPollRequest();
            }
            if (jObj.contains("ts")) {
                _ts = jObj.value("ts").toInt();
                doLongPollRequest();
            }

        }
   // }
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
    request.setRawHeader("User-Agent", "com.vk.vkclient/1654 (iPhone, iOS 12.2, iPhone8,4, Scale/2.0)");

    _manager->get(request);
}

void LongPoll::getLongPollHistoryRequest() {
    QUrlQuery query;
    query.addQueryItem("ts", QString::number(_ts));
    query.addQueryItem("pts", QString::number(_pts));
    query.addQueryItem("lp_version", "3");
    query.addQueryItem("access_token", _accessToken);
    query.addQueryItem("v", "5.93");
    QUrl url("https://api.vk.com/method/messages.getLongPollHistory");
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "com.vk.vkclient/1654 (iPhone, iOS 12.2, iPhone8,4, Scale/2.0)");
    _manager->get(request);
}

bool updatessort(const QVariant &a, const QVariant &b) {
    return a.toList().at(1).toInt()<b.toList().at(1).toInt();
}

void LongPoll::parseLongPollUpdates(QJsonArray updates) {
    QVariantList u = updates.toVariantList();
    qSort(u.begin(), u.end(), updatessort);
    updates = QJsonArray::fromVariantList(u);
    for (int index = 0; index < updates.size(); ++index) {
        QJsonArray update = updates.at(index).toArray();

        qDebug() << update << "\n";
        switch (update.at(0).toInt()) {
        case 4:
            delay(100);
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
