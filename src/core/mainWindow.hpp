#pragma once

#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QProgressDialog>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "window.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT

 private:
  std::unique_ptr<Window> _window;

  QSqlDatabase _db;

 public:
  MainWindow() : _window(std::make_unique<Window>()) {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = new QAction("Open", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);

    /**
     * Create the menu to support translation
     */
    QMenu* languageMenu = menuBar()->addMenu(tr("Language"));
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

    languageMenu->addAction(en_GBaction);
    languageMenu->addAction(esAction);
    languageMenu->addAction(frAction);
    languageMenu->addAction(deAction);

    QString exeDir = QCoreApplication::applicationDirPath();

    qDebug() << "executable path" << exeDir;

    QString dbPath = exeDir + "/../database/database.sqlite";

    if (QFile::exists(dbPath)) {
      // TODO: remove
      _db = QSqlDatabase::addDatabase("QSQLITE");
      _db.setDatabaseName(dbPath);

      if (_db.open()) {
        qDebug() << "Database is open!";
        _window->onChangeDB(generateRandomHash());
      } else {
        qDebug() << "Failed to open database!";
      }
    }

    setCentralWidget(_window.get());
  }

  /**
   * Allows for translation of page to chosen language
   */
  void changeLanguage(const QString& language) {
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
  };

 private:
  QString generateRandomHash() const {
    QByteArray randomData;
    randomData.resize(16);

    for (int i = 0; i < 16; ++i) {
      randomData[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    }

    QByteArray hash = QCryptographicHash::hash(randomData, QCryptographicHash::Sha256);

    return hash.toHex();
  }

 private slots:
  void openFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Select csv file",   // Dialog title
                                                    "",                  // Default directory
                                                    "CSV Files (*.csv)"  // File filters
    );

    if (!fileName.isEmpty()) {
      QProcess process;
      QStringList arguments;

      QString exeDir = QCoreApplication::applicationDirPath();
      arguments << fileName << exeDir + "/../database/gen_db.sqlite";

      connect(&process, &QProcess::readyReadStandardOutput,
              [&process]() { qDebug() << process.readAllStandardOutput(); });

      connect(&process, &QProcess::readyReadStandardError,
              [&process]() { qDebug() << process.readAllStandardError(); });

      process.start(exeDir + "/migration", arguments);

      if (!process.waitForStarted()) {
        qDebug() << "Failed to start migration!";
        QMessageBox::warning(this, "Error", "Migration process failed.");
        return;
      }

      if (!process.waitForFinished()) {
        qDebug() << "Failed to finish migration!";
        QMessageBox::warning(this, "Error", "Migration process failed.");
        return;
      }

      QMessageBox::information(this, "Success", "Migration completed successfully!");

      _db.setDatabaseName("../database/gen_db.sqlite");
      if (!_db.isOpen()) {
        _db.open();
      }
      _window->onChangeDB(generateRandomHash());
    } else {
      QMessageBox::warning(this, "No File", "No file was selected.");
    }
  }
};