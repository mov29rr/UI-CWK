#include "pages/pollutantOverview.hpp"

class Window : public QWidget
{
private:
    static constexpr int
        SIDEBAR_STRETCH_PROPORTION = 1,
        CONTENT_STRETCH_PROPORTION = 5;
public:
    Window()
    {
        setObjectName("body");

        auto layout = new QHBoxLayout;

        auto pageSelector = new QListWidget;
        auto pagesStack = new QStackedWidget;

        pageSelector->setObjectName("pageSelector");

        const std::vector<Page*> pages
        {
            new PollutantOverviewPage,
            new PollutantOverviewPage,
            new PollutantOverviewPage,
            new PollutantOverviewPage,
            new PollutantOverviewPage
        };

        for(auto page : pages)
        {
            pagesStack->addWidget(page);
            auto item = new QListWidgetItem(page->title());
            pageSelector->addItem(item);
        }

        if(!pages.empty())
        {
            pageSelector->setCurrentRow(0);
        }

        connect(pageSelector, &QListWidget::currentRowChanged,
                pagesStack, &QStackedWidget::setCurrentIndex);

        layout->addWidget(pageSelector, SIDEBAR_STRETCH_PROPORTION);
        layout->addWidget(pagesStack, CONTENT_STRETCH_PROPORTION);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
    }
};