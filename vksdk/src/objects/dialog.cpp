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

#include "dialog.h"

Dialog::Dialog(QObject *parent) : QObject(parent) {
    qRegisterMetaType<Message*>("Message*");
}

Dialog::~Dialog() {
    delete _lastMessage;
}

Dialog *Dialog::fromJsonObject(QJsonObject object) {
    Dialog *dialog = new Dialog();
    dialog->setLastMessage(Message::fromJsonObject(object.value("message").toObject()));
    dialog->setIsChat(dialog->lastMessage()->chat());
    dialog->setUnread(object.contains("unread") || !dialog->lastMessage()->readState());
    return dialog;
}

bool Dialog::unread() const
{
    return _unread;
}

void Dialog::setUnread(bool unread)
{
    _unread = unread;
}

bool Dialog::isChat() const
{
    return _isChat;
}

void Dialog::setIsChat(bool isChat)
{
    _isChat = isChat;
}

Message *Dialog::lastMessage() const
{
    return _lastMessage;
}

void Dialog::setLastMessage(Message *lastMessage)
{
    _lastMessage = lastMessage;
}

