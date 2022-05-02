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

#include "video.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrlQuery>

Video::Video(QObject *parent) : QObject(parent)
{

}

Video *Video::fromJsonObject(QJsonObject object) {
    Video *video = new Video();
    QString strFromObj = QJsonDocument(object).toJson(QJsonDocument::Compact).toStdString().c_str();
    qDebug() << "Video: " << strFromObj << "\n";
    if (object.contains("id")) video->setId(object.value("id").toInt());
    if (object.contains("owner_id")) video->setOwnerId(object.value("owner_id").toInt());
    if (object.contains("duration")) video->setDuration(object.value("duration").toInt());
    if (object.contains("title")) video->setTitle(object.value("title").toString());
    if (object.contains("photo_130")) video->setPhoto130(object.value("photo_130").toString());
    if (object.contains("photo_320")) video->setPhoto320(object.value("photo_320").toString());
    if (object.contains("photo_640")) video->setPhoto640(object.value("photo_640").toString());
    QJsonArray photosizes = object.value("image").toArray();
    for (int i=0; i<photosizes.size(); i++) {
        QJsonObject photosizei = photosizes.at(i).toObject();
        switch (photosizei.value("height").toInt()) {

            case 120: {
                video->setPhoto130(photosizei.value("url").toString());
                break;
            }
            case 450: {
                video->setPhoto640(photosizei.value("url").toString());
                break;
            }

            default: {
                break;
            }

        }
    }

    if (object.contains("files")) {
        QJsonObject files = object.value("files").toObject();
        if (files.contains("mp4_240")) video->setMp4240(files.value("mp4_240").toString());
        if (files.contains("mp4_360")) video->setMp4360(files.value("mp4_360").toString());
        if (files.contains("mp4_480")) video->setMp4480(files.value("mp4_480").toString());
        if (files.contains("mp4_720")) {
            qDebug() << " containsnum ";
            video->setMp4720(files.value("mp4_720").toString());
        }

        if (files.contains("external")) {
            QString external_url = files.value("external").toString();
            if (external_url.contains("watch")) {
            QString external_id = external_url.right(external_url.length()-external_url.indexOf(QString("?v="))-3);
            qDebug() << external_id << "\n";
            QUrl urll("https://iteroni.com/api/v1/videos/"+external_id);
            qDebug() << "QUrl: " << urll.toString() + "\n";
            QNetworkRequest request(urll);
            QNetworkAccessManager* _manager = new QNetworkAccessManager();
           // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
            QNetworkReply *reply = _manager->get(request);
            QEventLoop looppp;
            QObject::connect(reply, SIGNAL(finished()) , &looppp, SLOT(quit()));
            looppp.exec();
             QByteArray dataaa = reply->readAll();
             QString DataAsString     = QString::fromUtf8(dataaa);
           //  qDebug() << DataAsString;
             QJsonDocument jDoc = QJsonDocument::fromJson(dataaa);
             QJsonObject jObj = jDoc.object();
             if (DataAsString.contains("formatStreams")) {
                 qDebug() << "contains formatStreams";
                 QJsonArray videoformats = jObj.value("formatStreams").toArray();
                 for (int i=0; i<videoformats.size(); i++) {
                     qDebug() << QString::number(i);
                     QJsonObject videoformati = videoformats.at(i).toObject();
                     switch (((videoformati.value("itag").toString()).toInt())) {
                     case 18: {
                         qDebug() << "360";
                         QString video360url = videoformati.value("url").toString();
                         qDebug() << "video360url: ";
                         video->setMp4360(video360url);
                          qDebug() << videoformati.value("url").toString();
                          break;
                     }
                     case 22: {
                         qDebug() << "720";
                          QString video720url = videoformati.value("url").toString();
                          qDebug() << "video720url: ";
                         video->setMp4720(video720url);
                         qDebug() << video720url;
                         break;
                     }


                     }
                 }
             } else {
                  video->setExternal(files.value("external").toString());
             }
            } else {
                video->setExternal(files.value("external").toString());
            }
            video->setExternal(files.value("external").toString());
        }
    }
    return video;
}

int Video::id() const
{
    return _id;
}

void Video::setId(int id)
{
    _id = id;
}

int Video::ownerId() const
{
    return _ownerId;
}

void Video::setOwnerId(int ownerId)
{
    _ownerId = ownerId;
}

int Video::duration() const
{
    return _duration;
}

void Video::setDuration(int duration)
{
    _duration = duration;
}

QString Video::title() const
{
    return _title;
}

void Video::setTitle(const QString &title)
{
    _title = title;
}

void Video::setPhoto130(const QString &photo130)
{
    _photo130 = photo130;
}

void Video::setPhoto320(const QString &photo320)
{
    _photo320 = photo320;
}

void Video::setPhoto640(const QString &photo640)
{
    _photo640 = photo640;
}

QString Video::photo() const
{
    if (!_photo640.isEmpty()) return _photo640;
    if (!_photo320.isEmpty()) return _photo320;
    return _photo130;
}

void Video::setMp4240(const QString &mp4240)
{
    _mp4240 = mp4240;
}

void Video::setMp4360(const QString &mp4360)
{
    _mp4360 = mp4360;
}

void Video::setMp4480(const QString &mp4480)
{
    _mp4480 = mp4480;
}

void Video::setMp4720(const QString &mp4720)
{
    _mp4720 = mp4720;
}

void Video::setExternal(const QString &external)
{
    _external = external;
}

QString Video::video() const
{
    if (!_external.isEmpty()) return _external;
    if (!_mp4720.isEmpty()) return _mp4720;
    if (!_mp4480.isEmpty()) return _mp4480;
    if (!_mp4360.isEmpty()) return _mp4360;
    return _mp4240;
}

bool Video::external() {
    return !_external.isEmpty();
}

