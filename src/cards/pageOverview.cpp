#include "pageOverview.hpp"

PageOverviewCard::PageOverviewCard(QWidget* parent, const QStackedWidget* stack, Page* page)
    : QWidget(parent)
    , _stack(stack)
    , _page(page)
{
    auto layout = new QHBoxLayout;

    auto card = new QWidget;
    card->setObjectName("card");

    auto cardLayout = new QVBoxLayout;
    auto cardTitle = new QLabel(page->title);
    auto pageOverview = _page->overview();

    cardTitle->setObjectName("title");

    cardLayout->addWidget(cardTitle);
    cardLayout->addWidget(pageOverview, 1);

    cardLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    cardLayout->setAlignment(pageOverview, Qt::AlignCenter);

    card->setLayout(cardLayout);

    layout->addWidget(card);

    setLayout(layout);
}

void PageOverviewCard::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked(_stack->indexOf(_page));
    }

    QWidget::mousePressEvent(event);
}
