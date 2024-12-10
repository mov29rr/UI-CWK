#include <iostream>
#include "card.hpp"

Card::Card(QWidget* parent, const QStackedWidget* stack, Page* page)
    : QWidget(parent)
    , _stack(stack)
    , _page(page)
{
    auto layout = new QHBoxLayout;

    auto card = new QWidget;
    card->setObjectName("card");

    auto cardLayout = new QHBoxLayout;

    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(new QLabel(page->title));

    card->setLayout(cardLayout);

    layout->addWidget(card);

    setLayout(layout);
}

void Card::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked(_stack->indexOf(_page));
    }

    QWidget::mousePressEvent(event);
}
