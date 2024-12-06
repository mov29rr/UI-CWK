#include "persistentOrganicPollutants.hpp"

#include "core/dbConnection.hpp"

PersistentOrganicPollutantsPage::PersistentOrganicPollutantsPage() : Page("Persistent Organic Pollutants")
{
    DbConnection db("../database/database.sqlite");
    auto records = db.query
    (
        "SELECT DISTINCT id, label FROM determinand "
        "WHERE label LIKE \"PCB %\" "
        "LIMIT 100"
    );

    for(auto record : records)
    {
        _pcbIdentifiers.emplace_back
            ( record.field("id").value().toInt()
            , record.field("label").value().toString().toStdString()
        );
    }

    auto pcb = _pcbIdentifiers.front();

    records = db.query
    (
        "SELECT result FROM measurement "
        "WHERE determinand_id = :id "
        "LIMIT 100",
        [&pcb](QSqlQuery& query)
        {
            query.bindValue(":id", pcb.id);
        }
    );

    for(auto record : records)
    {
        std::cout
            << record.field("result").value().toReal()
            << record.field("units").value().toString().toStdString()
            << std::endl;
    }

    std::vector<QPointF> points;
    points.emplace_back(0, 6);
    points.emplace_back(2, 4);
    points.emplace_back(4, 8);
    points.emplace_back(6, 4);
    points.emplace_back(8, 5);
    points.emplace_back(10, 1);
    points.emplace_back(12, 3);
    points.emplace_back(14, 6);
    points.emplace_back(16, 3);
    points.emplace_back(18, 2);
    auto chart = new ComplianceColourCodedLineGraph
        ( "Persistent Organig Pollutants"
        , "x-axis"
        , "y-axis"
        , { 0, 20 }
        , { 0, 10 }
        , {
            .veryLow = 2,
            .low = 3,
            .high = 6,
            .veryHigh = 8
        }
        , points
    );

    layout->addWidget(chart->view());

    attachLayout();
}