#pragma once

#include <QString>
#include <QDateTime>

class RssItem
{
public:
    RssItem() = default;

    RssItem(QString title,
            QString link,
            QString description,
            QDateTime pubDate);

    ~RssItem() = default;

    const QString& title() const;
    const QString& link() const;
    const QString& description() const;
    const QDateTime& pubDate() const;

    void setTitle(const QString& title);
    void setLink(const QString& link);
    void setDescription(const QString& description);
    void setPubDate(const QDateTime& pubDate);

    // Optional utility
    bool isValid() const;

private:
    QString m_title;
    QString m_link;
    QString m_description;
    QDateTime m_pubDate;
};
