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

#ifndef LONGPOLL_H
#define LONGPOLL_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

#include <QNetworkConfigurationManager>

#include <QDebug>

class LongPoll : public QObject
{
    Q_OBJECT

public:
    explicit LongPoll(QObject *parent = 0);
    ~LongPoll();

    void setAccessToken(QString value);

    Q_INVOKABLE void getLongPollServer();

signals:
    void gotNewMessage(int id);
    void unreadDialogsCounterUpdated(int value);
    void readMessages(qint64 peerId, qint64 localId, bool out);
    void userTyping(qint64 userId, qint64 chatId);

public slots:
    void finished(QNetworkReply *reply);
    void configurationChanged(const QNetworkConfiguration&);

private:
    QString _accessToken;
    QString _server;
    QString _key;
    int _ts;
    int _pts;

    QNetworkConfigurationManager *_netcfg_manager;
    QNetworkAccessManager *_manager;

    QString _net_name;

    void doLongPollRequest();
    void getLongPollHistoryRequest();
    void parseLongPollUpdates(QJsonArray updates);
    void setOffline(bool offline = true);
};

#endif // LONGPOLL_H
