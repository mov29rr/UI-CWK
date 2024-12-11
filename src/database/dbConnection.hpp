#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include <iostream>
#include <functional>
#include <cassert>

/**
 * The database query binding structure.
 */
struct QueryBinding
{
    QString fieldName;
    QVariant value;
};

/**
 * The database connection structure.
 */
class DbConnection
{
private:
    QSqlDatabase _connection;
public:
    /**
     * Constructs a database connection.
     * 
     * @param path the connection string.
     */
    DbConnection(const QString& path);

    /**
     * Applies a query.
     * 
     * @param queryString the query string to query.
     * @param bindings an array of query bindings to apply.
     * 
     * @return The results of the query.
     */
    std::vector<QSqlRecord> query(const QString& queryString, const std::vector<QueryBinding>& bindings = {});
};