#include <QtCharts>

#include "core/pollutantContamination.hpp"

class ComplianceOverviewChart : public QWidget {
 public:
  ComplianceOverviewChart(int goodSamples, int mediumSamples, int badSamples) {

    setFixedSize(300, 200);

    auto chart = new QChart;
    auto layout = new QVBoxLayout;

    auto barSeries = new QBarSeries;

    auto
      goodSeries = new QBarSet("Good"),
      mediumSeries = new QBarSet("Medium"),
      badSeries = new QBarSet("Bad");
    
    goodSeries->setColor(GREEN);
    mediumSeries->setColor(AMBER);
    badSeries->setColor(RED);

    *goodSeries << goodSamples;
    *mediumSeries << mediumSamples;
    *badSeries << badSamples;

    barSeries->append(goodSeries);
    barSeries->append(mediumSeries);
    barSeries->append(badSeries);

    chart->addSeries(barSeries);

    auto view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(view);

    setLayout(layout);
  }
};