#pragma once

#include <QtWidgets>

/**
 * Abstract base class for pages within the app
 */
class Page : public QWidget {
  Q_OBJECT
 public:
  /// The page title
  const char* const title;

 private:
  QVBoxLayout* _layout;

 protected:
  /// The page content layout
  QVBoxLayout* content;
  // Menu bar
  QMenuBar* menuBar;

  /**
   * Constructs the page
   *
   * @param title the page title.
   */
  Page(const char* title);

  /**
   * Create the menu bar to support translation
   */
  void createMenuBar();

  /**
   * Allows for translation of page to chosen language
   */
  void changeLanguage(const QString& language);
};