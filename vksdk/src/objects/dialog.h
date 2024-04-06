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

#ifndef DIALOG_H
#define DIALOG_H

#include <QJsonObject>
#include <QObject>

#include "chat.h"
#include "message.h"
#include "user.h"

class Dialog : public QObject
{
    Q_OBJECT

public:
    explicit Dialog(QObject *parent = 0);
    ~Dialog();

    static Dialog *fromJsonObject(QJsonObject object);

    bool unread() const;
    void setUnread(bool unread);

    bool muted() const;
    void setMuted(bool muted);

    bool isChat() const;
    void setIsChat(bool isChat);

    Message *lastMessage() const;
    void setLastMessage(Message *lastMessage);

private:
    bool _unread;
    bool _muted;
    bool _isChat;
    Message *_lastMessage;
};

#endif // DIALOG_H
