#include "pages/pollutantOverview.hpp"

class Window : public QWidget
{
private:
    static constexpr int
        MIN_WIDTH = 600,
        MIN_HEIGHT = 400,
        SIDEBAR_MIN_WIDTH = 180,
        SIDEBAR_STRETCH_PROPORTION = 1,
        CONTENT_STRETCH_PROPORTION = 5;
public:
    Window()
    {
        setMinimumWidth(MIN_WIDTH);
        setMinimumHeight(MIN_HEIGHT);

        auto layout = new QHBoxLayout;

        auto pageSelector = new QListWidget;
        auto pagesStack = new QStackedWidget;

        pageSelector->setMinimumWidth(SIDEBAR_MIN_WIDTH);

        const std::vector<Page*> pages
        {
            new PollutantOverviewPage,
        };

        for(auto page : pages)
        {
            pagesStack->addWidget(page);

            auto item = new QListWidgetItem(page->title());
            pageSelector->addItem(item);
        }

        connect(pageSelector, &QListWidget::currentRowChanged,
                pagesStack, &QStackedWidget::setCurrentIndex);

        layout->addWidget(pageSelector, SIDEBAR_STRETCH_PROPORTION);
        layout->addWidget(pagesStack, CONTENT_STRETCH_PROPORTION);
        setLayout(layout);
    }
};