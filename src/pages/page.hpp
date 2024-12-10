#pragma once

#include <QtWidgets>

/**
 * Abstract base class for pages within the app
 */
class Page : public QWidget
{
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
     * @param title the page title.
     */
    Page(const char* title);
};