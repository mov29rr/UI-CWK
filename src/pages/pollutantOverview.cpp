#include "pollutantOverview.hpp"

PollutantOverviewPage::PollutantOverviewPage() : Page("Pollutant Overview Page")
{
  	// create searchbar
  	auto searchLabel = new QLabel(tr("Search:"));
  	auto searchField = new QLineEdit();
  	searchLabel->setBuddy(searchField);

  	auto searchLayout = new QHBoxLayout();
  	searchLayout->addWidget(searchLabel);
  	searchLayout->addWidget(searchField);

  	layout->addLayout(searchLayout);
  	layout->addSpacing(10);

  	// divider
  	auto divider1 = new QFrame();
  	divider1->setFrameShape(QFrame::HLine);
  	divider1->setFrameShadow(QFrame::Sunken);

  	layout->addWidget(divider1);

  	// information about pollutant depending on the pollutant chosen
  	auto pollutantLabel = new QLabel(tr("Pollutant"));
  	pollutantLabel->setAlignment(Qt::AlignLeft);

  	layout->addWidget(pollutantLabel);
  	layout->addSpacing(10);

  	auto info = new QTextBrowser();
  	info->setText(QObject::tr(
      	"Pollutant Information, Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris vel libero imperdiet, "
      	"auctor nisi non, ornare velit. Donec non ex condimentum velit accumsan porta. Morbi sed elementum augue. Cras "
      	"eleifend aliquet odio, in faucibus nunc finibus a. Morbi ac sagittis turpis. Sed imperdiet sed nisi at dapibus. "
      	"Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Praesent felis sem, "
    	"pharetra sed gravida eu, luctus nec leo. Integer sit amet auctor mi, sit amet placerat ligula."));
  	info->setReadOnly(true);
  	info->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

  	layout->addWidget(info);

  	auto divider2 = new QFrame();
  	divider2->setFrameShape(QFrame::HLine);
  	divider2->setFrameShadow(QFrame::Sunken);

  	layout->addSpacing(15);
  	layout->addWidget(divider2);
}