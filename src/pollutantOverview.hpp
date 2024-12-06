#pragma once

#include <QWidget>

class QLabel;
class QAction;
class QMenu;
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QFrame;
class QTextBrowser;
class QLocale;
class QStatusbar;
class QMenu;

class PollutantOverview : public QWidget
{
    Q_OBJECT
public:
    PollutantOverview();

private:
    void createTitleandSearchBar();
    void createStatusBar();
    void createMenu();
    void createDescription();
    void createChart();
    void arrangeLayout();
    void changeLanguage(const QString &language);

    QVBoxLayout *pollutantOverviewLayout;

    QHBoxLayout *titleSearchBar;
    QHBoxLayout *pollutantDescription;

    QLabel *titleLabel;
    QLabel *searchLabel;
    QLabel *pollutantLabel;
    QLabel *lang;

    QTextBrowser *info;

    QLineEdit *searchField;

    QWidget *centralWidget;

    QMenu *menu;

    QAction *esAction;
    QAction *frAction;
    QAction *deAction;
    QAction *en_GBaction;

    QFrame *line;

    QStatusBar *status;
};
