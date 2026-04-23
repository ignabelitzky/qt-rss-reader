#pragma once

#include <QString>
#include <QDate>

class RssItem
{
public:
    RssItem();
    RssItem(const QString& title,
            const QString& link,
            const QString& description,
            const QDate& pubDate);
    ~RssItem() = default;

    QString title() const;
    QString link() const;
    QString description() const;
    QDate pubDate() const;

    void setTitle(const QString& title);
    void setLink(const QString& link);
    void setDescription(const QString& description);
    void setPubDate(const QDate& pubDate);

private:
    QString m_title;
    QString m_link;
    QString m_description;
    QDate m_pubDate;
};
