#include "page.hpp"

class PollutantOverviewPage : public Page
{
public:
    PollutantOverviewPage()
    {
        auto layout = new QVBoxLayout;
        layout->setAlignment(Qt::AlignTop);

        auto header = new QLabel(title());
        header->setObjectName("header");
        layout->addWidget(header);
        layout->setAlignment(header, Qt::AlignCenter);

        setLayout(layout);
    }

    const char* title() override
    {
        return "Pollutant Overview Page";
    }
};