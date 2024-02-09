#include "commentsmodel.h"

CommentsModel::CommentsModel(QObject *parent) : QAbstractListModel(parent)
{}


int CommentsModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return _comments.size();
}

QVariant CommentsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    Comment * comment = _comments.at(index.row());
    switch (role) {
    case AvatarRole:
        return QVariant(_getAvatarSource(comment->fromId()));
    case TitleRole:
        return QVariant(_getTitle(comment->fromId()));
    case TextRole:
        return QVariant(comment->text());
    case DateRole:
        return QVariant(comment->date());
    case LikeCountRole:
        return QVariant(comment->likesCount());
    case UserLikedRole:
        return QVariant(comment->userLiked());
    case AttachmentsRole:
        return QVariant();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CommentsModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[AvatarRole] = "avatarSource";
    roles[TitleRole] = "title";
    roles[TextRole] = "commentText";
    roles[DateRole] = "datetime";
    roles[LikeCountRole] = "likeCont";
    roles[UserLikedRole] = "userLiked";
    roles[AttachmentsRole] = "attachments";
    return roles;
}

void CommentsModel::clear() {
    qDebug() << "clearing...";
    beginRemoveRows(QModelIndex(), 0, _comments.size());
    _comments.clear();
    _profiles.clear();
    _groups.clear();
    endRemoveRows();

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void CommentsModel::addGroup(Group *group) {
    if (_groups.contains(group->id())) return;

    _groups[group->id()] = group;

    QModelIndex startIndex = createIndex(0, 0, static_cast<void *>(0));
    QModelIndex endIndex = createIndex(_comments.size(), 0, static_cast<void *>(0));
    emit dataChanged(startIndex, endIndex);
}

void CommentsModel::addComment(Comment *comment) {
    beginInsertRows(QModelIndex(), _comments.size(), _comments.size());
    _comments.append(comment);
    qDebug() << comment->text();
    endInsertRows();

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void CommentsModel::addUser(User *user) {
    if (_profiles.contains(user->id())) return;

    _profiles[user->id()] = user;

    QModelIndex startIndex = createIndex(0, 0, static_cast<void *>(0));
    QModelIndex endIndex = createIndex(_comments.size(), 0, static_cast<void *>(0));
    emit dataChanged(startIndex, endIndex);
}

int CommentsModel::size() const {
    return _comments.size();
}

QString CommentsModel::_getAvatarSource(const int id) const {
    if (id > 0) {
        if (_profiles.contains(id)) return _profiles[id]->getPhoto50();
    } else if (id < 0) {
        if (_groups.contains(id)) return _groups[id]->photo50();
    }
    return "image://theme/icon-m-person";
}

QString CommentsModel::_getTitle(const int id) const {
    if (id > 0) {
        if (_profiles.contains(id))
            return QString("%1 %2").arg(_profiles[id]->getFirstName()).arg(_profiles[id]->getLastName());
    } else if (id < 0) {
        if (_groups.contains(id)) return _groups[id]->name();
    }
    return "";
}
