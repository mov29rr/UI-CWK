#pragma once

#include "core/window.hpp"

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

  virtual void onMount(const QString hash) {};

 private:
  virtual QWidget* overview() { return new QWidget; }

  friend class PageOverviewCard;
};