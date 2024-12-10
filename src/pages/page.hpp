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

  virtual void onMount(const QString hash) {};

 protected:
  /// The page layout
  QVBoxLayout* layout;

  QString _hash;

  bool toMount(const QString hash) {
    if (hash == _hash) {
      return false;
    }

    _hash = hash;
    return true;
  }

 public:
  /**
   * Constructs the page
   *
   * @param title the page title.
   */
  Page(const char* title);
};