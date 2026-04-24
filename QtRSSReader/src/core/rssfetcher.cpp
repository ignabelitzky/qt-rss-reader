#include "rssfetcher.h"
#include "rssparser.h"
#include <QNetworkReply>
#include <QNetworkRequest>

RssFetcher::RssFetcher(QObject *parent)
    : QObject{parent}
{
    connect(&m_manager, &QNetworkAccessManager::finished,
            this, &RssFetcher::onReplyFinished);
}

void RssFetcher::fetch(int feedId, const QUrl& url)
{
    if (!url.isValid())
    {
        emit errorOcurred(feedId, "Invalid URL");
        return;
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "QtRSSReader/1.0");

    QNetworkReply* reply = m_manager.get(request);
    reply->setProperty("feedId", feedId);
}

void RssFetcher::onReplyFinished(QNetworkReply* reply)
{
    int feedId = reply->property("feedId").toInt();

    if (reply->error() != QNetworkReply::NoError)
    {
        emit errorOcurred(feedId, reply->errorString());
        reply->deleteLater();
        return;
    }

    const QByteArray data = reply->readAll();
    QVector<RssItem> items = RssParser::parse(data);

    emit feedReady(feedId, items);

    reply->deleteLater();
}
