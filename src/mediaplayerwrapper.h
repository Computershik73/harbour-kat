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

#ifndef MEDIAPLAYERWRAPPER_H
#define MEDIAPLAYERWRAPPER_H

#include <time.h>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>

#include "playlistmodel.h"
#include "vksdk/src/objects/audio.h"

#include <QDebug>

class MediaPlayerWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY stateChanged)
    Q_PROPERTY(int currentIndex READ currentIndex CONSTANT)
    Q_PROPERTY(int size READ size CONSTANT)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY mediaChanged)
    Q_PROPERTY(QString author READ author NOTIFY mediaChanged)
    Q_PROPERTY(QString title READ title NOTIFY mediaChanged)
    Q_PROPERTY(qint64 ownerId READ ownerId CONSTANT)
    Q_PROPERTY(qint64 audioId READ audioId CONSTANT)
    Q_PROPERTY(PlaylistModel* model READ model CONSTANT)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle)
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat)

public:
    explicit MediaPlayerWrapper(QObject *parent = 0);
    ~MediaPlayerWrapper();

    Q_INVOKABLE void setPlaylist(QVariantList audios, int index);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void next();
    Q_INVOKABLE void prev();
    Q_INVOKABLE void seekTo(int value);
    Q_INVOKABLE void jumpTo(int index);
     bool isPlaying() const;
     bool isPaused() const;
     bool isStopped() const;
    qint64 position();
    int currentIndex() const;
    int size() const;
    qint64 duration();
    QString author();
    QString title();
    qint64 ownerId();
    qint64 audioId();

    PlaylistModel *model() const;

    bool shuffle() const;
    void setShuffle(bool shuffle);

    bool repeat() const;
    void setRepeat(bool repeat);

signals:
    void mediaChanged();
    void playlistChanged();
    void positionChanged(qint64 position);
    void stateChanged();

public slots:
    void _mediaChanged(QMediaContent content);
    void _positionChanged(qint64 pos);
    void _stateChanged(QMediaPlayer::State state);

private:
    QMediaPlayer *_player;
    QList<Audio*> _audios;
    PlaylistModel *_model;
    bool _shuffle = false;
    bool _repeat = false;
    bool _shuffleNow = false;
    int _currIndex;
};

#endif // MEDIAPLAYERWRAPPER_H
