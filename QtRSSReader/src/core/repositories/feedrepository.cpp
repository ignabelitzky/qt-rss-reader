#include "feedrepository.h"
#include "src/db/databasemanager.h"
#include <QSqlError>

FeedRepository::FeedRepository()
{
}

QList<Feed> FeedRepository::getAll()
{
    QList<Feed> feeds;

    QSqlQuery query(DatabaseManager::database());
    query.prepare("SELECT id, url, name, enabled, last_fetched FROM feeds");

    if (!query.exec())
    {
        qCritical() << "FeedRepository::getAll failed:" << query.lastError().text();
        return feeds;
    }

    while (query.next())
    {
        feeds.append(mapQueryToFeed(query));
    }

    return feeds;
}

Feed FeedRepository::getById(int id)
{
    QSqlQuery query(DatabaseManager::database());
    query.prepare("SELECT id, url, name, enabled, last_fetched FROM feeds WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qCritical() << "FeedRepository::getById failed:" << query.lastError().text();
        return {};
    }

    if (query.next())
    {
        return mapQueryToFeed(query);
    }

    return {};
}

bool FeedRepository::add(const Feed& feed)
{
    QSqlQuery query(DatabaseManager::database());
    query.prepare(R"(
        INSERT INTO feeds (url, name, enabled, last_fetched)
        VALUES (:url, :name, :enabled, :last_fetched)
    )");
    query.bindValue(":url", feed.url);
    query.bindValue(":name", feed.name);
    query.bindValue(":enabled", feed.enabled ? 1 : 0);

    if (feed.lastFetched.isValid())
    {
        query.bindValue(":last_fetched", feed.lastFetched);
    }
    else
    {
        query.bindValue(":last_fetched", QVariant(QVariant::DateTime));
    }

    if (!query.exec())
    {
        qCritical() << "FeedRepository::add failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool FeedRepository::update(const Feed& feed)
{
    QSqlQuery query(DatabaseManager::database());
    query.prepare(R"(
        UPDATE feeds
        SET url = :url, name = :name, enabled = :enabled, last_fetched = :last_fetched
        WHERE id = :id
    )");

    query.bindValue(":id", feed.id);
    query.bindValue(":url", feed.url);
    query.bindValue(":name", feed.name);
    query.bindValue(":enabled", feed.enabled ? 1 : 0);

    if (feed.lastFetched.isValid())
    {
        query.bindValue(":last_fetched", feed.lastFetched);
    }
    else
    {
        query.bindValue(":last_fetched", QVariant(QVariant::DateTime));
    }

    if (!query.exec())
    {
        qCritical() << "FeedRepository::update failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool FeedRepository::remove(int id)
{
    QSqlQuery query(DatabaseManager::database());
    query.prepare("DELETE FROM feeds WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qCritical() << "FeedRepository::remove failed:" << query.lastError().text();
        return false;
    }

    return true;
}

Feed FeedRepository::mapQueryToFeed(const QSqlQuery& query)
{
    Feed feed;

    feed.id = query.value("id").toInt();
    feed.url = query.value("url").toString();
    feed.name = query.value("name").toString();
    feed.enabled = query.value("enabled").toInt() == 1;

    QVariant dt = query.value("last_fetched");
    if (dt.isValid() && !dt.isNull())
    {
        feed.lastFetched = dt.toDateTime();
    }

    return feed;
}
