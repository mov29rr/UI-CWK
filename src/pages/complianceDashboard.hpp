#pragma once

#include <QComboBox>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

#include "pages/page.hpp"
#include "charts/complianceOverviewChart.hpp"

const int ROW_LIMIT = 200;

class ComplianceDashboardPage : public Page {
 public:
  ComplianceDashboardPage(QWidget *parent = nullptr);
  ~ComplianceDashboardPage();

  void onMount(const QString hash) override;

 private:
  struct Card {
    QLabel title;
    QLabel detail;
  };

  struct Pollutant {
    QString label;
    QString unit;
    QString info;
    qreal min;
    qreal max;
  };

  struct Measurement {
    QString location_name;
    uint pollutant_id;
    QString pollutant_name;
    qreal value;
    QString date;
    QString unit;
    bool status;
  };

  enum ComplianceStatus { ALL, GOOD, MEDIUM, BAD, NONE };

  void setupUI();
  void applyFilters();

  // New method to calculate compliance status
  ComplianceStatus calculateComplianceStatus(const Measurement &sample);
  void populateStats(const QString &bestLocation, const QString &worstLocation, const QString &bestYear,
                     const QString &worstYear, const QString &bestPollutant, const QString &worstPollutant,
                     int totalSamples, int missingDataCount, int compliantSamples, int mediumSamples,
                     int nonCompliantSamples);

  std::unordered_map<uint, Pollutant> m_pollutants;
  std::vector<Measurement> m_measurements;
  Card m_cards[4];
  QWidget *centralWidget;
  QVBoxLayout *mainLayout;
  QHBoxLayout *filtersLayout;
  QHBoxLayout *contentLayout;
  QGridLayout *cardsLayout;
  QTableWidget *detailedTable;
  QComboBox *yearFilter;
  QComboBox *locationFilter;
  QComboBox *pollutantFilter;
  QComboBox *statusFilter;
  QPushButton *filterButton;
  QTextEdit *importantInfo;
  QLabel *footer;
  QFrame *summaryCards[4];

  QWidget* _overviewPlaceholder;
  QVBoxLayout* _overviewLayout;
  std::unique_ptr<ComplianceOverviewChart> _overviewChart;

  int compliantSamples = 0, mediumSamples = 0, nonCompliantSamples = 0;

  int m_offset = 0;

  QWidget* overview() override;

 protected slots:
  void onScroll(int value) {
    QScrollBar *scrollBar = detailedTable->verticalScrollBar();
    if (scrollBar->value() == scrollBar->maximum()) {
      m_offset += ROW_LIMIT;
      applyFilters();
    }
  }
};