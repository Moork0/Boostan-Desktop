#include "header/network.h"

Network::Network(QObject *parent) : QObject(parent)
{
    connect(&netaccman, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

Network::Network(QUrl url, QObject *parent) : Network {parent}
{
    this->url = url;
}

QHash<QByteArray, QByteArray> Network::getHeaders() const
{
    return headers;
}

void Network::setHeaders(const QHash<QByteArray, QByteArray> &value)
{
    headers = value;
}

QUrl Network::getUrl() const
{
    return url;
}

void Network::setUrl(const QUrl &value)
{
    url = value;
}

void Network::addHeader(const QByteArray &header, const QByteArray &value)
{
    this->headers[header] = value;
}

bool Network::post(const QByteArray& data)
{
    QNetworkRequest request(this->url);
    bool status{false};
    request.setAttribute(QNetworkRequest::CookieSaveControlAttribute, QNetworkRequest::Manual);
    setRequestHeader(request);
    QNetworkReply *reply = netaccman.post(request, data);
    if (reply->error() == QNetworkReply::NoError) status = true;
    reply->deleteLater();
    return status;
}

bool Network::get()
{
    QNetworkRequest request(this->url);
    bool status{false};
    request.setAttribute(QNetworkRequest::CookieSaveControlAttribute, QNetworkRequest::Manual);
    setRequestHeader(request);
    QNetworkReply* reply = netaccman.get(request);
    if (reply->error() == QNetworkReply::NoError) status = true;
    reply->deleteLater();
    return status;
}

void Network::finished(QNetworkReply* reply)
{
    emit complete(*reply);
}

void Network::setRequestHeader(QNetworkRequest &req)
{
    QHash<QByteArray, QByteArray>::const_iterator it = headers.constBegin();
    while (it != headers.constEnd()) {
        req.setRawHeader(it.key(), it.value());
        ++it;
    }
}
