#include "header/network.h"

Network::Network(QObject *parent) : QObject(parent)
{
    connect(&netaccman, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
//    netaccman.setProxy(QNetworkProxy(QNetworkProxy::Socks5Proxy, "127.0.0.1", 9050));
}

Network::Network(QUrl url, QObject *parent) : Network {parent}
{
    this->url = url;
//    connect(&netaccman, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
//    netaccman.setProxy(QNetworkProxy(QNetworkProxy::Socks5Proxy, "127.0.0.1", 9050));
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
    setRequestHeader(request);
    QNetworkReply *reply = netaccman.post(request, data);
    return reply->error() == QNetworkReply::NoError;
}

bool Network::get()
{
    QNetworkRequest request(this->url);
    setRequestHeader(request);
    QNetworkReply* reply = netaccman.get(request);
    return reply->error() == QNetworkReply::NoError;
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
