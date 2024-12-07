#include "persistentOrganicPollutants.hpp"

PersistentOrganicPollutantsPage::PersistentOrganicPollutantsPage()
    : Page("Persistent Organic Pollutants")
    , _db("../database/database.sqlite")
{
    auto records = _db.query
    (
        "SELECT DISTINCT id, label FROM determinand "
        "WHERE LABEL LIKE \"PCB %\""
    );

    for(auto record : records)
    {
        _pcbs.emplace_back
            ( record.field("ID").value().toInt()
            , record.field("LABEL").value().toString()
            , record.field("UNIT").value().toString()
        );
    }

    // TODO: Select PCB
    auto pcb = _pcbs.front();

    // TODO: Select time frame, max points

    const QDateTime
        startDate(QDate(2024, 1, 1), QTime(0, 0, 0)),
        endDate(QDate(2025, 1, 1), QTime(0, 0, 0));

    records = _db.query
    (
        "SELECT RESULT, DATE FROM measurement "
        "WHERE DETERMINAND_ID = :id "
        "AND DATE BETWEEN :startDate AND :endDate "
        "ORDER BY DATE ASC "
        "LIMIT :maxMeasurements"
        , std::vector<Binding>
        {
            { ":id", pcb.id },
            { ":startDate", QDateTime(QDate(2024, 1, 1), QTime(0, 0, 0)) },
            { ":endDate", QDateTime(QDate(2025, 1, 1), QTime(0, 0, 0)) },
            { ":maxMeasurements", 100 }
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