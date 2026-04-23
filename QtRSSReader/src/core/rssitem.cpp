#include "rssitem.h"
#include <utility>

// Constructor
RssItem::RssItem(QString title,
                 QString link,
                 QString description,
                 QDateTime pubDate)
    : m_title(std::move(title))
    , m_link(std::move(link))
    , m_description(std::move(description))
    , m_pubDate(std::move(pubDate))
{
}

// Getters
const QString& RssItem::title() const
{
    return m_title;
}

const QString& RssItem::link() const
{
    return m_link;
}

const QString& RssItem::description() const
{
    return m_description;
}

const QDateTime& RssItem::pubDate() const
{
    return m_pubDate;
}

// Setters
void RssItem::setTitle(const QString& title)
{
    m_title = title;
}

void RssItem::setLink(const QString& link)
{
    m_link = link;
}

void RssItem::setDescription(const QString& description)
{
    m_description = description;
}

void RssItem::setPubDate(const QDateTime& pubDate)
{
    m_pubDate = pubDate;
}

// Optional utility
bool RssItem::isValid() const
{
    return !m_title.isEmpty() && !m_link.isEmpty();
}
