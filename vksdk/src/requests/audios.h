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

#ifndef AUDIOS_H
#define AUDIOS_H

#include "requestbase.h"

class Audios : public RequestBase
{
    Q_OBJECT
public:
    explicit Audios(QObject *parent = 0);

    Q_INVOKABLE void add(qint64 ownerId, qint64 audioId);
    Q_INVOKABLE void get(qint64 ownerId = 0, int count = 6000);
    Q_INVOKABLE void getRecommendations(int count = 50);
    Q_INVOKABLE void search(QString query);
};

#endif // AUDIOS_H
