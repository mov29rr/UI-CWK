#pragma once

#include "page.hpp"

#include "charts/complianceColourCodedLineChart.hpp"

/**
 * The persistent organic pollutants page class
 */
class PersistentOrganicPollutantsPage : public Page
{
public:
    /**
     * Constructs the persistent organic pollutants page
     */
    PersistentOrganicPollutantsPage() : Page("Persistent Organic Pollutants")
    {
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
        auto chart = new ComplianceColouredChart
            ( "Persistent Organig Pollutants"
            , points
            , { 0, 20 }
            , { 0, 10 }
            , {
                .veryLow = 2,
                .low = 3,
                .high = 6,
                .veryHigh = 8
            }
        );

        layout->addWidget(chart->view());

        attachLayout();
    }
};