#ifndef NETWORK_H
#define NETWORK_H

/*
    * Class: Network
    * Files: network.h and network.cpp
    * This class is a wrapper on ther QNetwork classes.
*/

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkProxy>

#include <QDebug>

#include "settings.h"
#include "../helpers/logger.h"

class Network : public QObject
{
    Q_OBJECT

private:
    QUrl url;
    QHash<QByteArray, QByteArray> headers;
    QNetworkAccessManager netaccman;

    // actually sets header for a request 'req' from 'headers'
    void setRequestHeader(QNetworkRequest& req);

public:
    explicit Network(QObject *parent = nullptr);
    explicit Network(QUrl url, QObject* parent = nullptr);

    // return a QHash which keys is the header titles and values are header values
    QHash<QByteArray, QByteArray> getHeaders() const;
    // replace 'value' as new headers
    void setHeaders(const QHash<QByteArray, QByteArray> &value);

    // return url
    QUrl getUrl() const;
    // set url
    void setUrl(const QUrl &value);

    // add a single header to headers with title 'header' and value 'value'
    void addHeader(const QByteArray& header, const QByteArray& value);
    // send a POST request to 'url' with data 'data'
    bool post(const QByteArray& data);
    // send a GET request to 'url'
    bool get();

signals:
    void complete(QNetworkReply& data);

private slots:
    // slot that connects to QNetworkReply finished
    void finished(QNetworkReply* reply);
};

#endif // NETWORK_H
