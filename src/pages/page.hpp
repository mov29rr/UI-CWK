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
    Page(const char* title)
        : title(title)
        , layout(new QVBoxLayout)
    {
        layout->setAlignment(Qt::AlignTop);

        auto header = new QLabel(title);
        header->setObjectName("header");
        layout->addWidget(header);
        layout->setAlignment(header, Qt::AlignCenter);
    }

    /**
     * Attaches a layout to the page
     */
    void attachLayout()
    {
        setLayout(layout);
    }
};