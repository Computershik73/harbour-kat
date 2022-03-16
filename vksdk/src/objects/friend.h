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

#ifndef FRIEND_H
#define FRIEND_H

#include <QJsonObject>
#include <QObject>
#include <QScopedPointer>
#include <QString>

#include <QDebug>

class Friend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString firstName READ firstName CONSTANT)
    Q_PROPERTY(QString lastName READ lastName CONSTANT)
    Q_PROPERTY(QString photo50 READ photo50 CONSTANT)
    Q_PROPERTY(bool online READ online CONSTANT)
    Q_PROPERTY(QString status READ status CONSTANT)

public:
    explicit Friend(QObject *parent = 0);
    ~Friend();

    static Friend* fromJsonObject(QJsonObject object, bool isGroup=false);

    int id() const;
    void setId(int id);

    QString firstName() const;
    void setFirstName(const QString &firstName);

    QString lastName() const;
    void setLastName(const QString &lastName);

    QString photo50() const;
    void setPhoto50(const QString &photo50);

    bool online() const;
    void setOnline(bool online);

    QString status() const;
    void setStatus(const QString &status);

private:
    int _id;
    QString _firstName;
    QString _lastName;
    QString _photo50;
    bool _online;
    QString _status;
};

#endif // FRIEND_H
