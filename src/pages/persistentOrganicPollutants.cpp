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
            , record.field("label").value().toString()
            , record.field("unit").value().toString()
        );
    }

    auto pcb = _pcbIdentifiers.front();

    records = db.query
    (
        "SELECT result, date FROM measurement "
        "WHERE determinand_id = :id "
        "LIMIT 100",
        [&pcb](QSqlQuery& query)
        {
            query.bindValue(":id", pcb.id);
        }
    );

    std::vector<PollutantContaminationGraph::Point> measurements;
    int i = 0;
    for(auto record : records)
    {
        measurements.emplace_back
            ( record.field("date").value().toDateTime()
            , record.field("result").value().toReal()
        );
    }

    auto chart = new PollutantContaminationGraph
        ( "Persistent Organig Pollutants"
        , { 0, 20 }
        , { 0, 10 }
        , {
            .veryLow = 2,
            .low = 3,
            .high = 6,
            .veryHigh = 8
        }
        , measurements
    );

    layout->addWidget(chart->view());

    attachLayout();
}