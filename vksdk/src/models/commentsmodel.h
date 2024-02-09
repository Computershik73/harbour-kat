#ifndef COMMENTSMODEL_H
#define COMMENTSMODEL_H

#include <QAbstractListModel>

#include "../objects/comment.h"
#include "../objects/group.h"
#include "../objects/user.h"

#include <QDebug>

class CommentsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int size READ size CONSTANT)

public:
    enum Roles {
        AvatarRole = Qt::UserRole + 1,
        TitleRole,
        TextRole,
        DateRole,
        LikeCountRole,
        UserLikedRole,
        AttachmentsRole,
    };

    CommentsModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void addComment(Comment *comment);
    Q_INVOKABLE void addGroup(Group *group);
    Q_INVOKABLE void addUser(User *user);

    int size() const;

private:
    QList<Comment *> _comments;
    QHash<int, User*> _profiles;
    QHash<int, Group*> _groups;

    QString _getAvatarSource(const int id) const;
    QString _getTitle(const int id) const;
};

#endif // COMMENTSMODEL_H
