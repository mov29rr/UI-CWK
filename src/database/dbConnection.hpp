#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include <iostream>
#include <functional>
#include <cassert>

struct Binding
{
    QString fieldName;
    QVariant value;
};

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

    std::vector<QSqlRecord> query(const QString& queryString, const std::vector<Binding>& bindings = {})
    {
        QSqlQuery query(_connection);
        query.prepare(queryString);
        
        for(const Binding& binding : bindings)
        {
            query.bindValue(binding.fieldName, binding.value);
        }

        assert(query.exec() && "Failed to query database");
        
        std::vector<QSqlRecord> records;

        while(query.next())
        {
            records.push_back(query.record());
        }

        return records;
    }
};