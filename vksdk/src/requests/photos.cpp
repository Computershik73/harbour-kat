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

#include <QFile>
#include <QTextCodec>
#include <QHttpMultiPart>


#include "photos.h"

Photos::Photos(QObject *parent) : RequestBase(parent)
{}

void Photos::getMessagesUploadServer() {
    _api->makeApiGetRequest("photos.getMessagesUploadServer", QUrlQuery(), ApiRequest::PHOTOS_GET_MESSAGES_UPLOAD_SERVER);
}

void Photos::uploadPhotoToServer(const QString &server, const QString &album, const QString &user, const QString &p) {
    Q_UNUSED(album)
    Q_UNUSED(user)

    QString path = p;
    path = path.remove("file://");
    QString fileType = path.split(".").last();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QFile file(tr(path.toUtf8()));
    if (file.open(QIODevice::ReadOnly)) {
        QHttpMultiPart *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                            QVariant(QString("image/%1").arg(fileType == "jpg" ? "jpeg" : fileType)));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant(QString("form-data; name=\"photo\"; filename=\"%1.%2\"").arg(qrand()).arg(fileType)));
        imagePart.setBody(file.readAll());
        multipart->append(imagePart);
        _api->makePostRequest(QUrl(server), QUrlQuery(), multipart, ApiRequest::PHOTOS_UPLOAD_TO_SERVER);
        file.close();
    }
}

void Photos::saveMessagesPhoto(const QString &photo, const QString &server, const QString &hash) {
    QUrlQuery query;
    query.addQueryItem("photo", photo);
    query.addQueryItem("server", server);
    query.addQueryItem("hash", hash);
    _api->makeApiGetRequest("photos.saveMessagesPhoto", query, ApiRequest::PHOTOS_SAVE_MESSAGES_PHOTO);
}

void Photos::get(QString ownerId, QString albumId, int offset) {
    QUrlQuery query;
    query.addQueryItem("owner_id", ownerId);
    query.addQueryItem("album_id", albumId);
    query.addQueryItem("count", "21");
    query.addQueryItem("rev", "1");
    if (offset != 0) query.addQueryItem("offset", QString::number(offset));
    _api->makeApiGetRequest("photos.get", query, ApiRequest::PHOTOS_GET );
}

void Photos::getAlbums(QString ownerId) {
    QUrlQuery query;
    query.addQueryItem("owner_id", ownerId);
    query.addQueryItem("need_system", "1");
    _api->makeApiGetRequest("photos.getAlbums", query, ApiRequest::PHOTOS_GET_ALBUMS);
}

void Photos::getAll(QString ownerId, int offset) {
    QUrlQuery query;
    query.addQueryItem("owner_id", ownerId);
    query.addQueryItem("count", "21");
    if (offset != 0) query.addQueryItem("offset", QString::number(offset));
    _api->makeApiGetRequest("photos.getAll", query, ApiRequest::PHOTOS_GET_ALL);
}

//void Photos::attachImage(QString image, QString mode, int groupId) {
//    pathToImage = image.replace("file://", "");
////    pathToImage = image;
//    mMode = mode;
//    mGroupId = groupId;
//    if (mode == "MESSAGE") api_getMessagesUploadServer();
//    else if (mode == "WALL") api_getWallUploadServer();
//}

//void Photos::gotServer(QJsonValue value, ApiRequest::TaskType type) {
//    QJsonObject object = value.toObject();
//    uploadFileToServer(object.value("upload_url").toString());
//}

//void Photos::savedImage(QJsonValue value, ApiRequest::TaskType type) {
//    qDebug() << value;
//    QJsonObject object = value.toArray().at(0).toObject();
//    emit imageUploaded(QString("photo%1_%2").arg(QString::number(object.value("owner_id").toInt()),
//                                                 QString::number(object.value("id").toInt())));
//}

//void Photos::uploadedImage(QNetworkReply *reply) {
//    if (reply->error() == QNetworkReply::NoError) {
//        QString jsonData = reply->readAll();
//        qDebug() << jsonData;
//        QJsonObject object = QJsonDocument::fromJson(jsonData.toUtf8()).object();

//        QUrlQuery *query = new QUrlQuery();
//        if (mGroupId != 0) query->addQueryItem("group_id", QString::number(mGroupId));
//        query->addQueryItem("server", QString::number(object.value("server").toInt()));
//        query->addQueryItem("photo", object.value("photo").toString());
//        query->addQueryItem("hash", object.value("hash").toString());

//        ApiRequest *request = new ApiRequest(this);
//        connect(request, SIGNAL(gotResponse(QJsonValue,ApiRequest::TaskType)),
//                this, SLOT(savedImage(QJsonValue,ApiRequest::TaskType)));
//        request->setAccessToken(_accessToken);

//        if (mMode == "MESSAGE") request->makeApiGetRequest("photos.saveMessagesPhoto", query,
//                                                      ApiRequest::PHOTOS_SAVE_MESSAGES_PHOTO);
//        else if (mMode == "WALL") request->makeApiGetRequest("photos.saveWallPhoto", query,
//                                                        ApiRequest::PHOTOS_SAVE_WALL_PHOTO);
//    } else qDebug() << "Failture:" << reply->errorString();
//}

//void Photos::setAccessToken(const QString &accessToken) {
//    _accessToken = accessToken;
//}

//void Photos::uploadFileToServer(QString url) {
//    QFile *file = new QFile(pathToImage);
//    file->open(QIODevice::ReadOnly);
//    QHttpMultiPart *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
//    QHttpPart imagePart;
//    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/*"));
//    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"photo\"; filename=\"" + pathToImage.split("/").last() + "\""));
//    imagePart.setHeader(QNetworkRequest::ContentLengthHeader, file->size());
//    imagePart.setBody(file->readAll());
//    multipart->append(imagePart);

//    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadedImage(QNetworkReply*)));
//    connect(manager, SIGNAL(finished(QNetworkReply*)), manager, SLOT(deleteLater()));
//    QNetworkRequest req((QUrl(url)));
//    manager->post(req, multipart);
//    file->close();

////    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

////    QHttpPart imagePart;
////    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
////    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"photo\"; filename=\"" + pathToImage.split("/").last() + "\""));
////    QFile *file = new QFile(pathToImage);
////    file->open(QIODevice::ReadOnly);
////    imagePart.setBodyDevice(file);
////    file->setParent(multiPart);

////    multiPart->append(imagePart);

////    QNetworkRequest req((QUrl(url)));
////    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
////    multiPart->setParent(manager);

////    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadedImage(QNetworkReply*)));
////    connect(manager, SIGNAL(finished(QNetworkReply*)), manager, SLOT(deleteLater()));
////    manager->post(req, multiPart);
//}

//void Photos::api_getMessagesUploadServer() {
//    ApiRequest *request = new ApiRequest(this);
//    connect(request, SIGNAL(gotResponse(QJsonValue,ApiRequest::TaskType)),
//            this, SLOT(gotServer(QJsonValue,ApiRequest::TaskType)));
//    request->setAccessToken(_accessToken);
//    request->makeApiGetRequest("photos.getMessagesUploadServer", new QUrlQuery(), ApiRequest::PHOTOS_GET_MESSAGES_UPLOAD_SERVER);
//}

//void Photos::api_getWallUploadServer() {
//    QUrlQuery *query = new QUrlQuery();
//    if (mGroupId != 0) query->addQueryItem("group_id", QString::number(mGroupId));
//    ApiRequest *request = new ApiRequest(this);
//    connect(request, SIGNAL(gotResponse(QJsonValue,ApiRequest::TaskType)),
//            this, SLOT(gotServer(QJsonValue,ApiRequest::TaskType)));
//    request->setAccessToken(_accessToken);
//    request->makeApiGetRequest("photos.getWallUploadServer", query, ApiRequest::PHOTOS_GET_MESSAGES_UPLOAD_SERVER);
//}
