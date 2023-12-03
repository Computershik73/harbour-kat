#include <QFile>
#include <QTextCodec>
#include <QHttpMultiPart>
#include <QDebug>


#include "docs.h"

Docs::Docs(QObject *parent) : RequestBase(parent)
{}

void Docs::getMessagesUploadServer() {
    _api->makeApiGetRequest("docs.getMessagesUploadServer", QUrlQuery(), ApiRequest::DOCS_GET_MESSAGES_UPLOAD_SERVER);
}

void Docs::uploadDocToServer(const QString &server, const QString &p) {
    qDebug() << p;
    QString path = p;
    path = path.remove("file://");
    QString fileType = path.split(".").last();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QFile file(tr(path.toUtf8()));
    if (file.open(QIODevice::ReadOnly)) {
        QHttpMultiPart *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                            QVariant(QString("multipart/form-data; boundary=7d73991305de")));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant(QString("form-data; name=\"file\"; filename=\"%1.%2\"").arg(file.fileName().remove("."+fileType)).arg(fileType)));
        QByteArray f = file.readAll();
        qDebug() << f.size();
        imagePart.setBody(f);
        multipart->append(imagePart);
        _api->makePostRequest(QUrl(server), QUrlQuery(), multipart, ApiRequest::DOCS_UPLOAD_TO_SERVER);
        file.close();
    }
}

void Docs::saveMessagesDoc(const QString &photo) {
    QUrlQuery query;
    query.addQueryItem("file", photo);
    //query.addQueryItem("server", server);
    //query.addQueryItem("hash", hash);
    _api->makeApiGetRequest("docs.save", query, ApiRequest::DOCS_SAVE_MESSAGES_DOC);
}

void Docs::get(QString ownerId, QString albumId, int offset) {
    QUrlQuery query;
    query.addQueryItem("owner_id", ownerId);
    query.addQueryItem("album_id", albumId);
    query.addQueryItem("count", "21");
    query.addQueryItem("rev", "1");
    if (offset != 0) query.addQueryItem("offset", QString::number(offset));
    _api->makeApiGetRequest("photos.get", query, ApiRequest::PHOTOS_GET );
}
