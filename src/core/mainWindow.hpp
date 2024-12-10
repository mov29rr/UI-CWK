#pragma once

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

    // TODO: remove
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName("../database/database.sqlite");

    if (_db.open()) {
      qDebug() << "Database is open!";
      _window->onChangeDB(generateRandomHash());
    } else {
      qDebug() << "Failed to open database!";
    }

    setCentralWidget(_window.get());
  }

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
      arguments << fileName << "../database/gen_db.sqlite";

      connect(&process, &QProcess::readyReadStandardOutput,
              [&process]() { qDebug() << process.readAllStandardOutput(); });

      connect(&process, &QProcess::readyReadStandardError,
              [&process]() { qDebug() << process.readAllStandardError(); });

      process.start("./migration", arguments);

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