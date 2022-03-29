#include "account.h"

Account::Account(QObject *parent) : RequestBase(parent)
{}

void Account::setOnline() {
    QUrlQuery query;
    query.addQueryItem("voip", "0");
    _api->makeApiGetRequest("account.setOnline", query, ApiRequest::ACCOUNT_SET_ONLINE);
}


void Account::banUser(int id) {
    QUrlQuery query;
    query.addQueryItem("user_id", QString::number(id));
    _api->makeApiGetRequest("account.banUser", query, ApiRequest::ACCOUNT_BAN_USER);
}

void Account::unbanUser(int id) {
    QUrlQuery query;
    query.addQueryItem("user_id", QString::number(id));
    _api->makeApiGetRequest("account.unbanUser", query, ApiRequest::ACCOUNT_UNBAN_USER);
}
