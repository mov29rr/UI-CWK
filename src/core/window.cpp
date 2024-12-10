#include "window.hpp"

Window::Window()
{
	setObjectName("body");

    auto layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

	auto sidebar = new QListWidget;
	auto content = new QStackedWidget;

	sidebar->setObjectName("sidebar");
	content->setObjectName("content");

	const std::vector<Page*> pages
	{
		new PollutantOverviewPage,
		new PersistentOrganicPollutantsPage,
		new FlourinatedCompoundsPage
	};

	for (auto page : pages)
	{
		content->addWidget(page);
		auto item = new QListWidgetItem(page->title);
		sidebar->addItem(item);
	}

	if (!pages.empty())
	{
		sidebar->setCurrentRow(0);
	}

  	connect(sidebar, &QListWidget::currentRowChanged, content, &QStackedWidget::setCurrentIndex);

    layout->addWidget(sidebar);
    layout->addWidget(content, 1);

    setLayout(layout);
}