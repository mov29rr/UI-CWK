#pragma once

#include "page.hpp"

#include <QtCharts>

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
        auto chart = new QChart;
        chart->legend()->hide();
        // chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setTitle("TODO: Graph Title");

        auto line = new QLineSeries;

        *line
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

        auto boundingXAxis = new QLineSeries;
        *boundingXAxis << QPointF(0, 0) << QPointF(18, 0);

        auto area = new QAreaSeries(line, boundingXAxis);
        area->setName("Persistent Organic Pollutants");

        QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
        gradient.setColorAt(0, 0xFF0000);
        gradient.setColorAt(0.5, 0xFFFF00);
        gradient.setColorAt(1, 0x00FF00);
        gradient.setCoordinateMode(QGradient::ObjectMode);
        area->setBrush(gradient);

        auto xAxis = new QValueAxis;
        xAxis->setRange(0, 18);
        xAxis->setLabelFormat("%i");
        chart->addAxis(xAxis, Qt::AlignBottom);

        auto yAxis = new QValueAxis;
        yAxis->setRange(0, 6);
        yAxis->setLabelFormat("%i");
        chart->addAxis(yAxis, Qt::AlignLeft);

        chart->addSeries(area);
        area->attachAxis(xAxis);
        area->attachAxis(yAxis);

        auto chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(chartView);

        attachLayout();
    }
};