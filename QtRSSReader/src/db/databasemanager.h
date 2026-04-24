#pragma once

#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager() = default;
    ~DatabaseManager() = default;

    static bool initialize();
    static QSqlDatabase database();

private:
    static bool createTables();
};
