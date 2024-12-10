#include "window.hpp"

Window::Window() {
  setObjectName("body");

  auto layout = new QHBoxLayout;

  _sidebar = new QListWidget;
  _content = new QStackedWidget;

  _sidebar->setObjectName("sidebar");
  _content->setObjectName("content");

  const std::vector<Page*> pages{new PollutantOverviewPage,
                                 // new PersistentOrganicPollutantsPage,
                                 new FlourinatedCompoundsPage};

  for (auto page : pages) {
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
  layout->setContentsMargins(0, 0, 0, 0);
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
}
