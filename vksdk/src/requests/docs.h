#ifndef DOCS_H
#define DOCS_H

#include "requestbase.h"

class Docs : public RequestBase
{
    Q_OBJECT
public:
    explicit Docs(QObject *parent = 0);

    void getMessagesUploadServer();
    void uploadDocToServer(const QString &server, const QString &path);
    void saveMessagesDoc(const QString &doc);

    Q_INVOKABLE void get(QString ownerId, QString albumId, int offset = 0);

};

#endif // DOCS_H
