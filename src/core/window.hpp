#pragma once

#include <QtWidgets>

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