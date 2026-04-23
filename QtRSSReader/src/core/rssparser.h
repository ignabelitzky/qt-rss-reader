#pragma once

#include "rssitem.h"

class RssParser
{
public:
    RssParser() = default;
    ~RssParser() = default;

    static QVector<RssItem> parse(const QByteArray& data);

private:
    static QDateTime parseDate(const QString& dateString);
};
