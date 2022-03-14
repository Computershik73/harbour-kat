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

#include "friend.h"
#include <QJsonDocument>

Friend::Friend(QObject *parent) : QObject(parent)
{}

Friend::~Friend()
{}

Friend *Friend::fromJsonObject(QJsonObject object) {
    QString strFromObj = QJsonDocument(object).toJson(QJsonDocument::Compact).toStdString().c_str();
    qDebug() << "User: " << strFromObj << "\n";
    Friend *frnd = new Friend();
    frnd->setId(object.value("id").toInt());
    if (object.value("id").toInt()>0) {
    frnd->setFirstName(object.value("first_name").toString());
    frnd->setLastName(object.value("last_name").toString());
    if (object.contains("photo_50")) frnd->setPhoto50(object.value("photo_50").toString());
    if (object.contains("online")) frnd->setOnline(object.value("online").toInt() == 1);
    if (object.contains("status")) frnd->setStatus(object.value("status").toString());
    } else {
        if (object.contains("id")) frnd->setId(object.value("id").toInt());
        if (object.contains("name")) { frnd->setFirstName(object.value("name").toString());
        frnd->setLastName(QString(""));
        }
        if (object.contains("status")) frnd->setStatus(QString(""));
        if (object.contains("photo_50")) frnd->setPhoto50(object.value("photo_50").toString());
        if (object.contains("online")) frnd->setOnline(false);


    }
    return frnd;
}

int Friend::id() const
{
    return _id;
}

void Friend::setId(int id)
{
    _id = id;
}

QString Friend::firstName() const
{
    return _firstName;
}

void Friend::setFirstName(const QString &firstName)
{
    _firstName = firstName;
}

QString Friend::lastName() const
{
    return _lastName;
}

void Friend::setLastName(const QString &lastName)
{
    _lastName = lastName;
}

QString Friend::photo50() const
{
    return _photo50;
}

void Friend::setPhoto50(const QString &photo50)
{
    _photo50 = photo50;
}

bool Friend::online() const
{
    return _online;
}

void Friend::setOnline(bool online)
{
    _online = online;
}

QString Friend::status() const
{
    return _status;
}

void Friend::setStatus(const QString &status)
{
    _status = status;
}

