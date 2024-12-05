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
protected:
    /// The page layout
    QVBoxLayout* layout;

    /**
     * Constructs the page
     * 
     * @param title the page title.
     */
    Page(const char* title);

    /**
     * Attaches a layout to the page
     */
    void attachLayout();
};