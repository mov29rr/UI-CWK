#include "mainDashboard.hpp"

#include "cards/pageOverview.hpp"

MainDashboardPage::MainDashboardPage(QListWidget* pageList, QStackedWidget* pageStack, const std::vector<Page*>& pages) : Page("Dashboard")
{
    auto grid = new QWidget;
    auto gridLayout = new QVBoxLayout;
    auto
        row1 = new QHBoxLayout,
        row2 = new QHBoxLayout;

    for(int i = 0; i < pages.size(); ++i)
    {
        auto card = new PageOverviewCard(grid, pageStack, pages[i]);
        auto row = i / 2 ? row2 : row1;
        
        row->addWidget(card);

        connect
            ( card
            , &PageOverviewCard::clicked
            , pageList
            , [pageList](int index)
            {
                pageList->setCurrentRow(index);
            }
        );
        connect
            ( card
            , &PageOverviewCard::clicked
            , pageStack
            , &QStackedWidget::setCurrentIndex
        );
    }

    gridLayout->addLayout(row1);
    gridLayout->addLayout(row2);

    grid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    grid->setLayout(gridLayout);

    content->addWidget(grid);
}