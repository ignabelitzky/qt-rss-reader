#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include "rssitem.h"

class RssFetcher : public QObject
{
    Q_OBJECT
public:
    explicit RssFetcher(QObject *parent = nullptr);
    ~RssFetcher() = default;

    void fetch(const QUrl& url);

signals:
    void feedReady(const QVector<RssItem>& items);
    void errorOcurred(const QString& error);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_manager;
};
