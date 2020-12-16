#include "header/base/network.h"

Network::Network(QObject *parent) : QObject(parent)
{
    connect(&netaccman, &QNetworkAccessManager::finished, this, &Network::finished);
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

// send a POST request to 'url' with data 'data' and return request status
bool Network::post(const QByteArray& data)
{
    QNetworkRequest request(this->url);
    request.setAttribute(QNetworkRequest::CookieSaveControlAttribute, QNetworkRequest::Manual);
    setRequestHeader(request);
    QNetworkReply *reply = netaccman.post(request, data);
    return reply->error() == QNetworkReply::NoError;
}

// send a GET request to 'url' return request status
bool Network::get()
{
    QNetworkRequest request(this->url);
    request.setAttribute(QNetworkRequest::CookieSaveControlAttribute, QNetworkRequest::Manual);
    setRequestHeader(request);
    QNetworkReply* reply = netaccman.get(request);
    return reply->error() == QNetworkReply::NoError;
}

void Network::finished(QNetworkReply* reply)
{
    emit complete(*reply);
}

// actually sets a request 'req' headers to 'headers'
void Network::setRequestHeader(QNetworkRequest &req)
{
    QHash<QByteArray, QByteArray>::const_iterator it = headers.cbegin();
    while (it != headers.cend()) {
        req.setRawHeader(it.key(), it.value());
        ++it;
    }
}
