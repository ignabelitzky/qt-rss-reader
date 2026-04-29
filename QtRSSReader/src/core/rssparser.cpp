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

            if (!insideItem)
                continue;

            // ---- Common fields ----
            if (name == "title")
            {
                currentItem.setTitle(
                    xml.readElementText(QXmlStreamReader::IncludeChildElements));
            }
            else if (name == "link")
            {
                if (isAtom)
                {
                    auto attrs = xml.attributes();
                    QString rel = attrs.value("rel").toString();

                    // Prefer rel="alternate", or fallback if rel missing
                    if (!attrs.hasAttribute("rel") || rel == "alternate")
                    {
                        currentItem.setLink(attrs.value("href").toString());
                    }
                }
                else
                {
                    currentItem.setLink(
                        xml.readElementText(QXmlStreamReader::IncludeChildElements));
                }
            }
            else if (name == "description")
            {
                // Only set if empty (content:encoded has priority)
                if (currentItem.description().isEmpty())
                {
                    QString desc = xml.readElementText(QXmlStreamReader::IncludeChildElements);
                    currentItem.setDescription(desc.isEmpty() ? "(no description)" : desc);
                }
            }
            else if (name == "summary") // Atom support
            {
                if (currentItem.description().isEmpty())
                {
                    currentItem.setDescription(
                        xml.readElementText(QXmlStreamReader::IncludeChildElements));
                }
            }
            else if (name == "encoded" &&
                     xml.namespaceUri() == "http://purl.org/rss/1.0/modules/content/")
            {
                // Prefer full content over description
                currentItem.setDescription(
                    xml.readElementText(QXmlStreamReader::IncludeChildElements));
            }
            else if (name == "pubDate" || name == "updated" || name == "published")
            {
                currentItem.setPubDate(parseDate(xml.readElementText()));
            }
        }

        if (xml.isEndElement())
        {
            const QStringView name = xml.name();

            if (name == "item" || name == "entry")
            {
                insideItem = false;

                // Basic validation: must have at least a link
                if (currentItem.isValid() && !currentItem.link().isEmpty())
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
        return dt;

    dt = QDateTime::fromString(dateString, Qt::ISODate);
    if (dt.isValid())
        return dt;

    // Fallback with timezone support
    dt = QDateTime::fromString(dateString, "ddd, dd MMM yyyy HH:mm:ss t");
    if (dt.isValid())
        return dt;

    return {};
}
