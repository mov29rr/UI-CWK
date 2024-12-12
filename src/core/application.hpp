#pragma once

#include "mainWindow.hpp"

/**
 * The application class
 */
class Application {
 private:
  static constexpr const char* STYLE_SHEET_FILE_PATH = "stylesheets/stylesheet.qss";

  std::unique_ptr<QApplication> _handle;
  std::unique_ptr<MainWindow> _window;

  bool _errored = false;

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