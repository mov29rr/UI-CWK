#include "pollutantOverview.hpp"

#include <iostream>

PollutantOverviewPage::PollutantOverviewPage() : Page("Pollutant Overview Page") {
  // create searchbar
  auto searchLabel = new QLabel(tr("Search:"));
  auto searchField = new QLineEdit();
  searchLabel->setBuddy(searchField);

  auto searchLayout = new QHBoxLayout();
  searchLayout->addWidget(searchLabel);
  searchLayout->addWidget(searchField);

  content->addLayout(searchLayout);
  content->addSpacing(10);

  // divider
  auto divider1 = new QFrame();
  divider1->setFrameShape(QFrame::HLine);
  divider1->setFrameShadow(QFrame::Sunken);

  content->addWidget(divider1);

  // information about pollutant depending on the pollutant chosen
  auto pollutantLabel = new QLabel(tr("Pollutant"));
  pollutantLabel->setAlignment(Qt::AlignLeft);

  content->addWidget(pollutantLabel);
  content->addSpacing(10);
}