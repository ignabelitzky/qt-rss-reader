#include "rssparser.h"
#include <QXmlStreamReader>

QVector<RssItem> RssParser::parse(const QByteArray& data)
{
    QVector<RssItem> items;

    QXmlStreamReader xml(data);

    RssItem currentItem;
    bool insideItem = false;
    bool isAtom = false;

    while (!xml.atEnd())
    {
        xml.readNext();

        if (xml.isStartElement())
        {
            const QStringView name = xml.name();

            // Detect format
            if (name == "entry")
            {
                insideItem = true;
                isAtom = true;
                currentItem = RssItem();
            }
            else if (name == "item")
            {
                insideItem = true;
                isAtom = false;
                currentItem = RssItem();
            }

            if (insideItem)
            {
                if (name == "title")
                {
                    currentItem.setTitle(xml.readElementText());
                }
                else if (name == "link")
                {
                    if (isAtom)
                    {
                        auto attrs = xml.attributes();
                        if (attrs.hasAttribute("href"))
                        {
                            currentItem.setLink(attrs.value("href").toString());
                        }
                    }
                    else
                    {
                        currentItem.setLink(xml.readElementText());
                    }
                }
                else if (name == "description")
                {
                    currentItem.setDescription(xml.readElementText());
                }
                else if (name == "content:encoded")
                {
                    currentItem.setDescription(xml.readElementText());
                }
                else if (name == "pubDate" || name == "updated" || name == "published")
                {
                    currentItem.setPubDate(parseDate(xml.readElementText()));
                }
            }
        }

        if (xml.isEndElement())
        {
            const QStringView name = xml.name();

            if ((name == "item") || (name == "entry"))
            {
                insideItem = false;

                if (currentItem.isValid())
                {
                    items.append(currentItem);
                }
            }
        }
    }

    if (xml.hasError())
    {
        qWarning() << "XML error:" << xml.errorString();
    }

    return items;
}

QDateTime RssParser::parseDate(const QString& dateString)
{
    QDateTime dt = QDateTime::fromString(dateString, Qt::RFC2822Date);
    if (dt.isValid())
    {
        return dt;
    }

    dt = QDateTime::fromString(dateString, Qt::ISODate);
    if (dt.isValid())
    {
        return dt;
    }

    dt = QDateTime::fromString(dateString, "ddd, dd MMM yyyy HH:mm:ss");
    return dt;
}
