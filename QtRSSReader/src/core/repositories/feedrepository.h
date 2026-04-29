#pragma once

#include <QList>
#include <QSqlQuery>
#include "src/models/feed.h"

class FeedRepository
{
public:
    FeedRepository();

    QList<Feed> getAll();
    QList<Feed> getAllBySearchTerm(const QString& searchTerm);
    Feed getById(int id);

    bool add(const Feed& feed);
    bool update(const Feed& feed);
    bool remove(int id);

private:
    Feed mapQueryToFeed(const QSqlQuery& query);
};
