#include "rssitem.h"

RssItem::RssItem()
    : m_title(QString(""))
    , m_link(QString(""))
    , m_description(QString(""))
    , m_pubDate(QDate::currentDate())
{

}

RssItem::RssItem(const QString &title,
                 const QString &link,
                 const QString &description,
                 const QDate& pubDate)
    : m_title(title.trimmed())
    , m_link(link.trimmed())
    , m_description(description.trimmed())
    , m_pubDate(pubDate)
{

}

QString RssItem::title() const
{
    return m_title;
}

QString RssItem::link() const
{
    return m_link;
}

QString RssItem::description() const
{
    return m_description;
}

QDate RssItem::pubDate() const
{
    return m_pubDate;
}

void RssItem::setTitle(const QString &title)
{
    m_title = title.trimmed();
}

void RssItem::setLink(const QString &link)
{
    m_link = link.trimmed();
}

void RssItem::setDescription(const QString &description)
{
    m_description = description.trimmed();
}

void RssItem::setPubDate(const QDate &pubDate)
{
    m_pubDate = pubDate;
}
