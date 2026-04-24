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

    void fetch(int feedId, const QUrl& url);

signals:
    void feedReady(int feedId, const QVector<RssItem>& items);
    void errorOcurred(int feedId, const QString& error);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_manager;
};
