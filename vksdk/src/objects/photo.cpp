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

#include "photo.h"
#include <QJsonArray>
Photo::Photo(QObject *parent) : QObject(parent)
{}

Photo *Photo::fromJsonObject(QJsonObject object) {
    Photo *photo = new Photo();
    if (object.contains("id")) photo->setId(object.value("id").toInt());
    if (object.contains("album_id")) photo->setAlbumId(object.value("album_id").toInt());
    if (object.contains("owner_id")) photo->setOwnerId(object.value("owner_id").toInt());
    if (object.contains("text")) photo->setText(object.value("text").toString());
    if (object.contains("date")) photo->setDate(object.value("date").toInt());
    if (object.contains("photo_75")) photo->setPhoto75(object.value("photo_75").toString());
    if (object.contains("photo_130")) photo->setPhoto130(object.value("photo_130").toString());
    if (object.contains("photo_604")) photo->setPhoto604(object.value("photo_604").toString());
    if (object.contains("photo_807")) photo->setPhoto807(object.value("photo_807").toString());
    if (object.contains("photo_1280")) photo->setPhoto1280(object.value("photo_1280").toString());
    if (object.contains("photo_2560")) photo->setPhoto2560(object.value("photo_2560").toString());
    QJsonArray photosizes = object.value("sizes").toArray();
    for (int i=0; i<photosizes.size(); i++) {
        QJsonObject photosizei = photosizes.at(i).toObject();
        switch (photosizei.value("height").toInt()) {
            case 75: {
                photo->setPhoto75(photosizei.value("url").toString());
                break;
            }
            case 130: {
                photo->setPhoto130(photosizei.value("url").toString());
                break;
            }
            case 604: {
                photo->setPhoto604(photosizei.value("url").toString());
                break;
            }
            case 807: {
                photo->setPhoto807(photosizei.value("url").toString());
                break;
            }
            case 961: {
                photo->setPhoto1280(photosizei.value("url").toString());
                break;
            }
            case 1080: {
                photo->setPhoto1280(photosizei.value("url").toString());
                break;
            }
            case 1280: {
                photo->setPhoto1280(photosizei.value("url").toString());
                break;
            }
            case 2560: {
            photo->setPhoto2560(photosizei.value("url").toString());
            break;
            }
            default: {
                break;
            }

        }
    }
    /*QJsonObject photosize1 = photosizes.at(0).toObject();
    photo->setPhoto75(photosize1.value("url").toString());
    QJsonObject photosize2 = photosizes.at(1).toObject();
    photo->setPhoto130(photosize2.value("url").toString());
    QJsonObject photosize3 = photosizes.at(2).toObject();
    photo->setPhoto604(photosize3.value("url").toString());
    QJsonObject photosize4 = photosizes.at(3).toObject();
    photo->setPhoto807(photosize4.value("url").toString());
    QJsonObject photosize5 = photosizes.at(4).toObject();
    photo->setPhoto1280(photosize5.value("url").toString());
    QJsonObject photosize6 = photosizes.at(5).toObject();
    photo->setPhoto2560(photosize6.value("url").toString());*/
    return photo;
}

int Photo::id() const
{
    return _id;
}

void Photo::setId(int id)
{
    _id = id;
}

int Photo::albumId() const
{
    return _albumId;
}

void Photo::setAlbumId(int albumId)
{
    _albumId = albumId;
}

int Photo::ownerId() const
{
    return _ownerId;
}

void Photo::setOwnerId(int ownerId)
{
    _ownerId = ownerId;
}

int Photo::date() const
{
    return _date;
}

void Photo::setDate(int date)
{
    _date = date;
}

QString Photo::text() const
{
    return _text;
}

void Photo::setText(const QString &text)
{
    _text = text;
}

void Photo::setPhoto75(const QString &photo75)
{
    _photo75 = photo75;
}

void Photo::setPhoto130(const QString &photo130)
{
    _photo130 = photo130;
}

void Photo::setPhoto604(const QString &photo604)
{
    _photo604 = photo604;
}

void Photo::setPhoto807(const QString &photo807)
{
    _photo807 = photo807;
}

void Photo::setPhoto1280(const QString &photo1280)
{
    _photo1280 = photo1280;
}

void Photo::setPhoto2560(const QString &photo2560)
{
    _photo2560 = photo2560;
}

QString Photo::photoMinimum() const {
    if (!_photo75.isNull() && !_photo75.isEmpty()) return _photo75;
    if (!_photo130.isNull() && !_photo130.isEmpty()) return _photo130;
    if (!_photo604.isNull() && !_photo604.isEmpty()) return _photo604;
    if (!_photo807.isNull() && !_photo807.isEmpty()) return _photo807;
    if (!_photo1280.isNull() && !_photo1280.isEmpty()) return _photo1280;
    if (!_photo2560.isNull() && !_photo2560.isEmpty()) return _photo2560;
    return "";
}

QString Photo::photoMaximum() const {
    if (!_photo2560.isNull() && !_photo2560.isEmpty()) return _photo2560;
    if (!_photo1280.isNull() && !_photo1280.isEmpty()) return _photo1280;
    if (!_photo807.isNull() && !_photo807.isEmpty()) return _photo807;
    if (!_photo604.isNull() && !_photo604.isEmpty()) return _photo604;
    if (!_photo130.isNull() && !_photo130.isEmpty()) return _photo130;
    if (!_photo75.isNull() && !_photo75.isEmpty()) return _photo75;
    return "";
}

