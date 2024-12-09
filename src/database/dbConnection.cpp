#include "dbConnection.hpp"

DbConnection::DbConnection(const QString& path) : _connection(QSqlDatabase::addDatabase("QSQLITE"))
{
    _connection.setDatabaseName(path);

    if (!_connection.open())
    {
        throw std::runtime_error("Failed to connect to database");
    }
}

std::vector<QSqlRecord> DbConnection::query(const QString& queryString, const std::vector<QueryBinding>& bindings)
{
    QSqlQuery query(_connection);
    query.prepare(queryString);
    
    for(const auto& binding : bindings)
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