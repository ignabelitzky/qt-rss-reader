#include "databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool DatabaseManager::initialize()
{
    if (QSqlDatabase::contains("app_connection"))
    {
        return true;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "app_connection");
    db.setDatabaseName("rss_reader.db");

    if (!db.open())
    {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    return createTables();
}

QSqlDatabase DatabaseManager::database()
{
    return QSqlDatabase::database("app_connection");
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(database());

    const QString createFeedsTable = R"(
        CREATE TABLE IF NOT EXISTS feeds (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            url TEXT NOT NULL,
            name TEXT,
            enabled INTEGER DEFAULT 1,
            last_fetched DATETIME
        );
    )";

    if (!query.exec(createFeedsTable))
    {
        qCritical() << "Failed to create feeds table:" << query.lastError().text();
        return false;
    }

    return true;
}
