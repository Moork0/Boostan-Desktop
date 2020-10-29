#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkProxy>

#include <QDebug>

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    explicit Network(QUrl url, QObject* parent = nullptr);

    QHash<QByteArray, QByteArray> getHeaders() const;
    void setHeaders(const QHash<QByteArray, QByteArray> &value);

    QUrl getUrl() const;
    void setUrl(const QUrl &value);

    void addHeader(const QByteArray& header, const QByteArray& value);
    bool post(const QByteArray& data);
    bool get();

signals:
    void complete(QNetworkReply& data);

public slots:
    void finished(QNetworkReply* reply);

private:
    QUrl url;
    QHash<QByteArray, QByteArray> headers;
    QNetworkAccessManager netaccman;

    void setRequestHeader(QNetworkRequest& req);
};

#endif // NETWORK_H
