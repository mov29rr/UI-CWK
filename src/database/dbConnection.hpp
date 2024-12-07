#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include <iostream>
#include <functional>
#include <cassert>

class DbConnection
{
private:
    QSqlDatabase _connection;
public:
    DbConnection(const QString& path) : _connection(QSqlDatabase::addDatabase("QSQLITE"))
    {
        _connection.setDatabaseName(path);

        if (!_connection.open())
        {
            throw std::runtime_error("Failed to connect to database");
        }
    }

    std::vector<QSqlRecord> query(const QString& queryString, std::function<void(QSqlQuery&)> preQuery = [](QSqlQuery&){})
    {
        QSqlQuery query(_connection);
        query.prepare(queryString);

        preQuery(query);

        assert(query.exec() && "Failed to query database");
        
        std::vector<QSqlRecord> records;

        while(query.next())
        {
            records.push_back(query.record());
        }

        return records;
    }
};