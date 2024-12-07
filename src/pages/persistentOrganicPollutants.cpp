#include "persistentOrganicPollutants.hpp"

#include "database/dbConnection.hpp"

PersistentOrganicPollutantsPage::PersistentOrganicPollutantsPage() : Page("Persistent Organic Pollutants")
{
    DbConnection db("../database/database.sqlite");
    auto records = db.query
    (
        "SELECT DISTINCT id, label FROM determinand "
        "WHERE label LIKE \"PCB %\""
    );

    for(auto record : records)
    {
        _pcbs.emplace_back
            ( record.field("id").value().toInt()
            , record.field("label").value().toString()
            , record.field("unit").value().toString()
        );
    }

    // TODO: Select PCB
    auto pcb = _pcbs.front();

    // TODO: Select time frame, max points

    const QDateTime
        startDate(QDate(2024, 1, 1), QTime(0, 0, 0)),
        endDate(QDate(2025, 1, 1), QTime(0, 0, 0));

    records = db.query
    (
        "SELECT result, date FROM measurement "
        "WHERE determinand_id = :id "
        "AND date BETWEEN :startDate AND :endDate "
        "ORDER BY date ASC "
        "LIMIT :maxPoints ",
        [&pcb](QSqlQuery& query)
        {
            query.bindValue(":id", pcb.id);
            query.bindValue(":startDate", QDateTime(QDate(2024, 1, 1), QTime(0, 0, 0)));
            query.bindValue(":endDate", QDateTime(QDate(2025, 1, 1), QTime(0, 0, 0)));
            query.bindValue(":maxPoints", 100);
        }
    );

    std::vector<PollutantContaminationGraph::Point> measurements;
    for(auto record : records)
    {
        measurements.emplace_back
            ( record.field("date").value().toDateTime()
            , record.field("result").value().toReal()
        );
    }

    auto chart = new PollutantContaminationGraph
        ( "Persistent Organig Pollutants"
        , { startDate, endDate }
        , { 0, 30 }
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