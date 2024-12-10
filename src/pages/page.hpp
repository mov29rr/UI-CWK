#pragma once

#include "core/window.hpp"

/**
 * Abstract base class for pages within the app
 */
class Page : public QWidget
{
    Q_OBJECT
public:
    /// The page title
    const char* const title;
private:
    QVBoxLayout* _layout;
protected:
    /// The page content layout
    QVBoxLayout* content;

    /**
     * Constructs the page
     * 
     * @param window the parent window.
     * @param title the page title.
     */
    Page(Window* window, const char* title);

    // TODO: Make pure virtual
    virtual QWidget* overview() {}
};