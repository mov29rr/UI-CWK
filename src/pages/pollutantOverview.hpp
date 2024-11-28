#include "page.hpp"

class PollutantOverviewPage : public Page
{
public:
    PollutantOverviewPage()
    {
        // TODO: Use headers rather than labels
        auto layout = new QVBoxLayout;
        layout->setAlignment(Qt::AlignTop);

        auto title = new QLabel(this->title());
        layout->addWidget(title);
        layout->setAlignment(title, Qt::AlignCenter);

        setLayout(layout);
    }

    const char* title() override
    {
        return "Pollutant Overview Page";
    }
};