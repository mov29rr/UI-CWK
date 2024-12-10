#pragma once

#include "pages/flourinatedCompound.hpp"
#include "pages/page.hpp"
#include "pages/persistentOrganicPollutants.hpp"
#include "pages/pollutantOverview.hpp"

/**
 * The main window class
 */
class Window : public QWidget {
 private:
  QString _hash;

  QListWidget *_sidebar;
  QStackedWidget *_content;

 public:
  /**
   * Constructs the window
   */
  Window();

  void onChangeDB(const QString hash);
};