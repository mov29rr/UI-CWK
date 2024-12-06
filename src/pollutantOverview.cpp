#include "pollutantOverview.hpp"

#include <QtWidgets>

PollutantOverview::PollutantOverview() : QMainWindow()
{
    // vertical layout of the window
    pollutantOverviewLayout = new QVBoxLayout();

    createTitleandSearchBar();
    createStatusBar();
    createMenu();
    createDescription();
    createChart();

    changeLanguage("eng");
    arrangeLayout();

    setWindowTitle(tr("Water Quality Monitor"));
}

void PollutantOverview::createStatusBar()
{
    status = new QStatusBar();

    setStatusBar(status);

    pollutantOverviewLayout->addWidget(status);
}

void PollutantOverview::createMenu()
{
    esAction = new QAction(tr("es"), this);
    esAction->setStatusTip(tr("Translate this page to Spanish"));
    connect(esAction, &QAction::triggered, this, [this]()
            { changeLanguage("es"); });

    frAction = new QAction(tr("fr"), this);
    frAction->setStatusTip(tr("Translate this page to French"));
    connect(frAction, &QAction::triggered, this, [this]()
            { changeLanguage("fr"); });

    deAction = new QAction(tr("de"), this);
    frAction->setStatusTip(tr("Translate this page to German"));
    connect(frAction, &QAction::triggered, this, [this]()
            { changeLanguage("de"); });

    en_GBaction = new QAction(tr("en_GB"), this);
    en_GBaction->setStatusTip(tr("Translate this page to British English"));
    connect(en_GBaction, &QAction::triggered, this, [this]()
            { changeLanguage("en_GB"); });

    menu = menuBar()->addMenu(tr("&Lang"));
    menu->addAction(en_GBaction);
    menu->addAction(esAction);
    menu->addAction(frAction);
    menu->addAction(deAction);
}

void PollutantOverview::changeLanguage(const QString &language)
{
    QTranslator trn;

    if (language == "es")
    {
        if (trn.load("waterQuality_es.qm", ":/i18n"))
        {
            qApp->installTranslator(&trn);
        }
    }
    else if (language == "fr")
    {
        if (trn.load("waterQuality_fr.qm", ":/i18n"))
        {
            qApp->installTranslator(&trn);
        }
    }
    else if (language == "de")
    {
        if (trn.load("waterQuality_de.qm", ":/i18n"))
        {
            qApp->installTranslator(&trn);
        }
    }
    else if (language == "en_GB")
    {
        if (trn.load("waterQuality_en.qm", ":/i18n"))
        {
            qApp->installTranslator(&trn);
        }
    }
}

void PollutantOverview::createTitleandSearchBar()
{
    // create title
    titleLabel = new QLabel(tr("<h1>Pollutant Overview</h1>"));
    titleLabel->setAlignment(Qt::AlignLeft);

    // create searchbar
    searchLabel = new QLabel(tr("&Search:"));
    searchField = new QLineEdit();
    searchLabel->setBuddy(searchField);
    searchField->setAlignment(Qt::AlignCenter);

    // add line divider
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    // create horizontal layout for title and searchbar
    titleSearchBar = new QHBoxLayout();
    titleSearchBar->addWidget(titleLabel);
    titleSearchBar->addStretch();
    titleSearchBar->addWidget(searchLabel);
    titleSearchBar->addWidget(searchField);

    // add to vertical page layout
    pollutantOverviewLayout->addLayout(titleSearchBar);
    pollutantOverviewLayout->addWidget(line);
}

void PollutantOverview::createDescription()
{
    // information about pollutant depending on the pollutant chosen
    pollutantLabel = new QLabel(tr("Pollutant"));

    info = new QTextBrowser();
    info->setText(QObject::tr("Pollutant Information, Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris vel libero imperdiet, auctor nisi non, ornare velit. Donec non ex condimentum velit accumsan porta. Morbi sed elementum augue. Cras eleifend aliquet odio, in faucibus nunc finibus a. Morbi ac sagittis turpis. Sed imperdiet sed nisi at dapibus. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Praesent felis sem, pharetra sed gravida eu, luctus nec leo. Integer sit amet auctor mi, sit amet placerat ligula."));
    info->setReadOnly(true);
    info->setAlignment(Qt::AlignCenter);
    info->setMaximumSize(800, 200);

    pollutantDescription = new QHBoxLayout();
    pollutantDescription->addWidget(pollutantLabel);
    pollutantDescription->addWidget(info);
    pollutantDescription->setAlignment(Qt::AlignCenter);

    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    pollutantOverviewLayout->addWidget(pollutantLabel);
    pollutantOverviewLayout->addLayout(pollutantDescription);
    pollutantOverviewLayout->addWidget(line);
}

void PollutantOverview::createChart()
{
    // Implementation for creating search bar
}

void PollutantOverview::arrangeLayout()
{
    // layout for whole window
    centralWidget = new QWidget(this);
    centralWidget->setLayout(pollutantOverviewLayout);
    setCentralWidget(centralWidget);
}