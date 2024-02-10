#include "comment.h"

Comment::Comment(QObject *parent) : QObject(parent)
{}

Comment::~Comment() {
    qDeleteAll(_audios);
    qDeleteAll(_documents);
    qDeleteAll(_links);
    qDeleteAll(_photos);
    qDeleteAll(_videos);
    _audios.clear();
    _documents.clear();
    _links.clear();
    _photos.clear();
    _videos.clear();
}

Comment *Comment::fromJsonObject(QJsonObject object) {
    Comment *comment = new Comment();
    if (object.contains("id")) comment->setId(object.value("id").toInt());
    if (object.contains("from_id")) comment->setFromId(object.value("from_id").toInt());
    if (object.contains("date")) comment->setDate(object.value("date").toInt());
    if (object.contains("text")) comment->setText(object.value("text").toString());
    if (object.contains("reply_to_user")) comment->setReplyToUser(object.value("reply_to_user").toInt());
    if (object.contains("reply_to_comment")) comment->setReplyToComment(object.value("reply_to_comment").toInt());
    if (object.contains("likes")) {
        QJsonObject likes = object.value("likes").toObject();
        if (likes.contains("count")) comment->setLikesCount(likes.value("count").toInt());
        if (likes.contains("user_likes")) comment->setUserLiked(likes.value("user_likes").toInt() == 1);
    }
    if (object.contains("attachments")) {
        QJsonArray attachments = object.value("attachments").toArray();
        for (int index = 0; index < attachments.size(); ++index) {
            QJsonObject attachment = attachments.at(index).toObject();
            if (attachment.value("type").toString() == "photo") {
                comment->addPhoto(Photo::fromJsonObject(attachment.value("photo").toObject()));
            } else if (attachment.value("type").toString() == "posted_photo") {
                comment->addPhoto(Photo::fromJsonObject(attachment.value("photo").toObject()));
            } else if (attachment.value("type").toString() == "video") {
                comment->addVideo(Video::fromJsonObject(attachment.value("video").toObject()));
            } else if (attachment.value("type").toString() == "audio") {
                comment->addAudio(Audio::fromJsonObject(attachment.value("audio").toObject()));
            } else if (attachment.value("type").toString() == "doc") {
                comment->addDocument(Document::fromJsonObject(attachment.value("doc").toObject()));
            } else if (attachment.value("type").toString() == "graffiti") {
                //
            } else if (attachment.value("type").toString() == "link") {
                comment->addLink(Link::fromJsonObject(attachment.value("link").toObject()));
            } else if (attachment.value("type").toString() == "note") {
                //
            } else if (attachment.value("type").toString() == "app") {
                //
            } else if (attachment.value("type").toString() == "poll") {
                //
            } else if (attachment.value("type").toString() == "page") {
                //
            } else if (attachment.value("type").toString() == "album") {
                //
            } else if (attachment.value("type").toString() == "photos_list") {
                comment->addPhoto(Photo::fromJsonObject(attachment.value("photo").toObject()));
            } else if (attachment.value("type").toString() == "market") {
                //
            } else if (attachment.value("type").toString() == "market_album") {
                //
            }
        }
    }
    return comment;
}

int Comment::id() const
{
    return _id;
}

void Comment::setId(int id)
{
    _id = id;
}

int Comment::fromId() const
{
    return _fromId;
}

void Comment::setFromId(int fromId)
{
    _fromId = fromId;
}

int Comment::date() const
{
    return _date;
}

void Comment::setDate(int date)
{
    _date = date;
}

QString Comment::text() const
{
    return _text;
}

void Comment::setText(const QString &text)
{
    _text = text;
}

int Comment::replyToUser() const
{
    return _replyToUser;
}

void Comment::setReplyToUser(int replyToUser)
{
    _replyToUser = replyToUser;
}

int Comment::replyToComment() const
{
    return _replyToComment;
}

void Comment::setReplyToComment(int replyToComment)
{
    _replyToComment = replyToComment;
}

int Comment::likesCount() const
{
    return _likeCount;
}

void Comment::setLikesCount(int count) {
    _likeCount=count;
}

bool Comment::userLiked() const
{
    return _userLiked;
}

void Comment::setUserLiked(bool liked)
{
    _userLiked = liked;
}

QVariant Comment::audios() const
{
    return QVariant::fromValue(_audios);
}

QList<QObject *> Comment::audiosList() const
{
    return _audios;
}

void Comment::addAudio(Audio *audio)
{
    _audios.append(audio);
}

QVariant Comment::documents() const
{
    return QVariant::fromValue(_documents);
}

QList<QObject *> Comment::documentsList() const
{
    return _documents;
}

void Comment::addDocument(Document *document)
{
    _documents.append(document);
}

QVariant Comment::links() const
{
    return QVariant::fromValue(_links);
}

QList<QObject *> Comment::linksList() const
{
    return _links;
}

void Comment::addLink(Link *link)
{
    _links.append(link);
}

QVariant Comment::photos() const
{
    return QVariant::fromValue(_photos);
}

QList<QObject *> Comment::photosList() const
{
    return _photos;
}

void Comment::addPhoto(Photo *photo)
{
    _photos.append(photo);
}

QVariant Comment::videos() const
{
    return QVariant::fromValue(_videos);
}

QList<QObject *> Comment::videosList() const
{
    return _videos;
}

void Comment::addVideo(Video *video)
{
    _videos.append(video);
}
