#include "page.hpp"

Page::Page(const char* title)
    : title(title), _layout(new QVBoxLayout), content(new QVBoxLayout), menuBar(new QMenuBar) {
  // Create menu bar
  createMenuBar();

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

  _layout->addWidget(menuBar);
  _layout->addWidget(header);
  _layout->addLayout(content, 1);
  _layout->addWidget(footer);

  setLayout(_layout);
}

void Page::createMenuBar() {
  QAction* esAction = new QAction(tr("es"), this);
  // esAction->setStatusTip(tr("Translate this page to Spanish"));
  connect(esAction, &QAction::triggered, this, [this]() { changeLanguage("es"); });

  QAction* frAction = new QAction(tr("fr"), this);
  // frAction->setStatusTip(tr("Translate this page to French"));
  connect(frAction, &QAction::triggered, this, [this]() { changeLanguage("fr"); });

  QAction* deAction = new QAction(tr("de"), this);
  // frAction->setStatusTip(tr("Translate this page to German"));
  connect(frAction, &QAction::triggered, this, [this]() { changeLanguage("de"); });

  QAction* en_GBaction = new QAction(tr("en_GB"), this);
  // en_GBaction->setStatusTip(tr("Translate this page to British English"));
  connect(en_GBaction, &QAction::triggered, this, [this]() { changeLanguage("en_GB"); });

  QMenu* menu = menuBar->addMenu(tr("Language"));
  menu->addAction(en_GBaction);
  menu->addAction(esAction);
  menu->addAction(frAction);
  menu->addAction(deAction);
}

void Page::changeLanguage(const QString& language) {
  QTranslator trn;

  if (language == "es") {
    if (trn.load("translations/waterQuality_es.qm", ":/i18n")) {
      qApp->installTranslator(&trn);
    }
  } else if (language == "fr") {
    if (trn.load("translations/waterQuality_fr.qm", ":/i18n")) {
      qApp->installTranslator(&trn);
    }
  } else if (language == "de") {
    if (trn.load("translations/waterQuality_de.qm", ":/i18n")) {
      qApp->installTranslator(&trn);
    }
  } else if (language == "en_GB") {
    if (trn.load("translations/waterQuality_en.qm", ":/i18n")) {
      qApp->installTranslator(&trn);
    }
  }
}