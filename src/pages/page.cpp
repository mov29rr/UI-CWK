#include "page.hpp"

Page::Page(const char* title)
    : title(title)
    , layout(new QVBoxLayout)
{
    layout->setAlignment(Qt::AlignTop);

    auto header = new QLabel(title);
    header->setObjectName("header");
    layout->addWidget(header);
    layout->setAlignment(header, Qt::AlignCenter);
}

void Page::attachLayout()
{
    setLayout(layout);
}