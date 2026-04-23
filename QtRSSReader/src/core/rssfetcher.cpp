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

void RssFetcher::fetch(const QUrl& url)
{
    if (!url.isValid())
    {
        emit errorOcurred("Invalid URL");
        return;
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "QtRSSReader/1.0");

    m_manager.get(request);
}

void RssFetcher::onReplyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        emit errorOcurred(reply->errorString());
        reply->deleteLater();
        return;
    }

    const QByteArray data = reply->readAll();
    QVector<RssItem> items = RssParser::parse(data);

    emit feedReady(items);

    reply->deleteLater();
}
