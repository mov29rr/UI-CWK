#include "window.hpp"

#include "pages/mainDashboard.hpp"
#include "pages/pollutantOverview.hpp"
#include "pages/persistentOrganicPollutants.hpp"
#include "pages/flourinatedCompound.hpp"

Window::Window() {
  setObjectName("body");

  auto layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  _sidebar = new QListWidget;
  _content = new QStackedWidget;

  _sidebar->setObjectName("sidebar");
  _content->setObjectName("content");

	std::vector<Page*> pages
	{
		new PollutantOverviewPage,
		new PersistentOrganicPollutantsPage,
		new FlourinatedCompoundsPage
	};

	auto mainDashboard = new MainDashboardPage(_sidebar, _content, pages);

	pages.insert(pages.begin(), mainDashboard);

	for (auto page : pages)
	{
		_content->addWidget(page);
		auto item = new QListWidgetItem(page->title);
		_sidebar->addItem(item);
	}

  if (!pages.empty()) {
    _sidebar->setCurrentRow(0);
  }

  connect(_sidebar, &QListWidget::currentRowChanged, this, [this](int currentRow) {
    _content->setCurrentIndex(currentRow);

    QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);

    if (db.isValid() && db.isOpen()) {
      QWidget* widget = _content->widget(currentRow);
      if (widget) {
        auto customWidget = qobject_cast<Page*>(widget);
        if (customWidget) {
          customWidget->onMount(_hash);
        }
      }
    }
  });

  layout->addWidget(_sidebar);
  layout->addWidget(_content, 1);

  setLayout(layout);
}

void Window::onChangeDB(const QString hash) {
  _hash = hash;
  QWidget* widget = _content->widget(_sidebar->currentRow());
  if (widget) {
    auto customWidget = qobject_cast<Page*>(widget);
    if (customWidget) {
      customWidget->onMount(_hash);
    }
  }
};
