#ifndef COMMENT_H
#define COMMENT_H

#include <QJsonArray>
#include <QObject>

#include "audio.h"
#include "document.h"
#include "link.h"
#include "photo.h"
#include "video.h"

class Comment : public QObject
{
    Q_OBJECT

public:
    explicit Comment(QObject *parent = 0);
    ~Comment();

    static Comment *fromJsonObject(QJsonObject object);

    int id() const;
    void setId(int id);

    int fromId() const;
    void setFromId(int fromId);

    int date() const;
    void setDate(int date);

    QString text() const;
    void setText(const QString &text);

    int replyToUser() const;
    void setReplyToUser(int replyToUser);

    int replyToComment() const;
    void setReplyToComment(int replyToComment);

    int likesCount() const;
    void setLikesCount(int count);

    bool userLiked() const ;
    void setUserLiked(bool liked);


    QVariant audios() const;
    QList<QObject *> audiosList() const;
    void addAudio(Audio *audio);

    QVariant documents() const;
    QList<QObject *> documentsList() const;
    void addDocument(Document *document);

    QVariant links() const;
    QList<QObject *> linksList() const;
    void addLink(Link *link);

    QVariant photos() const;
    QList<QObject *> photosList() const;
    void addPhoto(Photo *photo);

    QVariant videos() const;
    QList<QObject *> videosList() const;
    void addVideo(Video *video);

private:
    int _id = 0;
    int _fromId = 0;
    int _date = 0;
    QString _text = 0;
    int _replyToUser = 0;
    int _replyToComment = 0;
    int _likeCount = 0;
    bool _userLiked = false;
    QList<QObject*> _audios;
    QList<QObject*> _documents;
    QList<QObject*> _links;
    QList<QObject*> _photos;
    QList<QObject*> _videos;
};

#endif // COMMENT_H
