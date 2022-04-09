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

#include "mediaplayerwrapper.h"

MediaPlayerWrapper::MediaPlayerWrapper(QObject *parent) : QObject(parent) {
    qsrand(time(NULL));
    _player = new QMediaPlayer(this);
    _player->stop();
    _model = new PlaylistModel(this);
    connect(_player, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(_mediaChanged(QMediaContent)));
    connect(_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(_stateChanged(QMediaPlayer::State)));
    connect(_player, SIGNAL(positionChanged(qint64)), this, SLOT(_positionChanged(qint64)));
    qRegisterMetaType<PlaylistModel*>("PlaylistModel*");
}

MediaPlayerWrapper::~MediaPlayerWrapper() {
    delete _player;
    delete _model;
}

void MediaPlayerWrapper::setPlaylist(QVariantList audios, int index) {
    _audios.clear();
    _model->clear();
    QMediaPlaylist *_playlist = new QMediaPlaylist();
    _playlist->setPlaybackMode(QMediaPlaylist::Loop);
    foreach (QVariant audio, audios) {
        Audio *_audio = (Audio*)audio.value<QObject*>();
        _playlist->addMedia(QUrl(_audio->url()));
        _audios.append(_audio);
        _model->add(_audio);
    }
    _player->setPlaylist(_playlist);
    if (index != -1) {
        _playlist->setCurrentIndex(index);
        _model->setPlayingIndex(index);
        _player->play();
    }
}

void MediaPlayerWrapper::play() {
    _player->play();
}

void MediaPlayerWrapper::pause() {
    _player->pause();
}

void MediaPlayerWrapper::next() {
    if (_shuffle) _player->playlist()->setCurrentIndex(qrand() % _audios.size());
    else {
        if (_player->playlist()->currentIndex()<=_audios.size()-1) {
        _player->playlist()->next();
        }
    }
}

void MediaPlayerWrapper::prev() {
    if (_shuffle) _player->playlist()->setCurrentIndex(qrand() % _audios.size());
    else _player->playlist()->previous();
}

void MediaPlayerWrapper::seekTo(int value) {
    _player->setPosition(value * 1000);
}

void MediaPlayerWrapper::jumpTo(int index) {
    _player->playlist()->setCurrentIndex(index);
}

qint64 MediaPlayerWrapper::position() {
    return _player->position();
}

int MediaPlayerWrapper::currentIndex() const {
    if (_player->playlist() == NULL) return -1;
    return _player->playlist()->currentIndex();
}

int MediaPlayerWrapper::size() const {
    return _audios.size();
}

qint64 MediaPlayerWrapper::duration() {
    if (_player->playlist() == NULL) return 0;
    return _audios.at(_player->playlist()->currentIndex())->duration();
}

QString MediaPlayerWrapper::author() {
    if (_player->playlist() == NULL) return "";
    return _audios.at(_player->playlist()->currentIndex())->artist();
}

QString MediaPlayerWrapper::title() {
    if (_player->playlist() == NULL) return "";
    return _audios.at(_player->playlist()->currentIndex())->title();
}

qint64 MediaPlayerWrapper::ownerId() {
    return _audios.at(_player->playlist()->currentIndex())->ownerId();
}

qint64 MediaPlayerWrapper::audioId() {
    return _audios.at(_player->playlist()->currentIndex())->id();
}

void MediaPlayerWrapper::_mediaChanged(QMediaContent content) {
    /*if (!((_player==NULL) || (_player->playlist()==NULL) || (content==NULL))) {
    if ((repeat()) || (_player->playlist()->currentIndex()>=_audios.size()-1)) {
        _player->playlist()->setCurrentIndex(_currIndex);
        emit mediaChanged();
    }
    else {
        if (_shuffle && _shuffleNow) {
            _shuffleNow = false;
            _player->playlist()->setCurrentIndex(qrand() % _audios.size());
        } else _shuffleNow = true;
        _currIndex = _player->playlist()->currentIndex();
        emit mediaChanged();
    }
    }*/
     emit mediaChanged();
}

void MediaPlayerWrapper::_positionChanged(qint64 pos) {
    emit positionChanged(pos);
}

void MediaPlayerWrapper::_stateChanged(QMediaPlayer::State state) {
    /*if (state == QMediaPlayer::State::StoppedState)
            {
                _player->play();
            }*/
    //_player->play();
    emit stateChanged();
}

bool MediaPlayerWrapper::repeat() const
{
    return _repeat;
}

void MediaPlayerWrapper::setRepeat(bool repeat)
{
    _repeat = repeat;
}

bool MediaPlayerWrapper::shuffle() const
{
    return _shuffle;
}

void MediaPlayerWrapper::setShuffle(bool shuffle)
{
    _shuffle = shuffle;
    _shuffleNow = true;
}

PlaylistModel *MediaPlayerWrapper::model() const {
    return _model;
}

bool MediaPlayerWrapper::isPlaying() const {
    return _player->state() == QMediaPlayer::PlayingState;
}

bool MediaPlayerWrapper::isPaused() const {
    return _player->state() == QMediaPlayer::PausedState;
}

bool MediaPlayerWrapper::isStopped() const {
    return _player->state() == QMediaPlayer::StoppedState;
}

