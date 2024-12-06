#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "window.hpp"

/**
 * The application class
 */
class Application {
 private:
  static constexpr const char* STYLE_SHEET_FILE_PATH = "stylesheets/stylesheet.qss";

  std::unique_ptr<QApplication> _handle;
  std::unique_ptr<Window> _window;

  bool _errored = false;

  QSqlDatabase m_db;

 public:
  /**
   * Constructs the application
   *
   * @param argc the argument count.
   * @param argv the argument vector.
   */
  Application(int argc, char** argv);

  /**
   * Executes the application
   */
  int exec();
};