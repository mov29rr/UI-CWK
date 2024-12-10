#include "page.hpp"

Page::Page(const char* title)
	: title(title)
	, _layout(new QVBoxLayout)
	, content(new QVBoxLayout)
{
	// Header
    auto header = new QWidget;
	auto headerLayout = new QHBoxLayout;
    header->setObjectName("header");

	auto headerTitle = new QLabel(title);
	headerLayout->addWidget(headerTitle);
	headerLayout->setAlignment(headerTitle, Qt::AlignCenter);

	header->setLayout(headerLayout);

	// Footer
	auto footer = new QWidget;
	auto footerLayout = new QHBoxLayout;
	footer->setObjectName("footer");

	auto footerText = new QLabel("TODO: Footer text");
	footerLayout->addWidget(footerText);

	footer->setLayout(footerLayout);

	// Contents
	content->setContentsMargins(10, 10, 10, 10);
	content->setAlignment(Qt::AlignTop);

	// Layout
	_layout->setContentsMargins(0, 0, 0, 0);
  	_layout->setAlignment(Qt::AlignTop);

    _layout->addWidget(header);
	_layout->addLayout(content, 1);
	_layout->addWidget(footer);

    setLayout(_layout);
}
