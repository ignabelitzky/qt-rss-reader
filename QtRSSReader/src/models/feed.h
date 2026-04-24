#pragma once

#include <QString>
#include <QDateTime>

struct Feed {
    int id;
    QString url;
    QString name;
    bool enabled;
    QDateTime lastFetched;
};
