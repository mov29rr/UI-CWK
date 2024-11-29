#pragma once

#include "page.hpp"

#include <QtCharts>

class PersistentOrganicPollutantsPage : public Page
{
public:
    /**
     * Constructs the persistent organic pollutants page
     */
    PersistentOrganicPollutantsPage() : Page("Persistent Organic Pollutants")
    {
        auto series = new QLineSeries;

        *series
            << QPointF(0, 6) 
            << QPointF(2, 4)
            << QPointF(4, 8)
            << QPointF(6, 4)
            << QPointF(8, 5)
            << QPointF(10, 1)
            << QPointF(12, 3)
            << QPointF(14, 6)
            << QPointF(16, 3)
            << QPointF(18, 2);

        auto chart = new QChart;
        chart->legend()->hide();
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setTitle("TODO: Graph Title");

        auto chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(chartView);

        attachLayout();
    }
};