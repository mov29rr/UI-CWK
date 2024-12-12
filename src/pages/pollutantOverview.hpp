#pragma once

#include <QLineEdit>
#include <QTextBrowser>

#include "page.hpp"

/**
 * The pollutant overview page.
 */
class PollutantOverviewPage : public Page {
 public:
  /**
   * Constructs the pollutant overview page.
   */
  PollutantOverviewPage();

 private:
  QWidget* overview() override { return new QWidget; }
};