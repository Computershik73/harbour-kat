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

#ifndef DIALOGSLISTMODEL_H
#define DIALOGSLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "../objects/chat.h"
#include "../objects/dialog.h"
#include "../objects/friend.h"

#include <QDebug>

class DialogsListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(qint64 size READ size CONSTANT)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        AvatarRole,
        TitleRole,
        PreviewRole,
        UnreadRole,
        IsChatRole,
        IsOnlineRole,
        IsOutRole,
    };

    explicit DialogsListModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void add(Dialog *dialog);
    Q_INVOKABLE void addProfile(Friend *profile);
    Q_INVOKABLE void addChat(Chat *chat);
    Q_INVOKABLE void readMessages(qint64 peerId, qint64 localId, bool out);
    Q_INVOKABLE void update(Message *message);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void updateModel();

    qint64 size() const;

private:
    QList<qint64> _dialogsIds;
    QHash<qint64, Chat*> _chats;
    QHash<qint64, Dialog*> _dialogs;
    QHash<qint64, Friend*> _profiles;
};

#endif // DIALOGSLISTMODEL_H
