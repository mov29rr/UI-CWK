#include "application.hpp"

Application::Application(int argc, char** argv)
    : _handle(std::make_unique<QApplication>(argc, argv)), _window(std::make_unique<Window>()) {
  try {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("../database/database.sqlite");
    if (!m_db.open()) {
      qDebug() << "Failed to open database:";
    }

    // Reading stylesheet from file
    QFile file(STYLE_SHEET_FILE_PATH);
    if (!file.exists()) {
      throw std::runtime_error("Failed to open stylesheet");
    }

    file.open(QFile::ReadOnly);
    auto contents = QLatin1String(file.readAll());

    _handle->setStyleSheet(contents);

    _window->show();

    // QSqlQuery query;
    // query.prepare("SELECT ID FROM site LIMIT :LIMIT");
    // query.bindValue(":LIMIT", 20);
    // if (query.exec()) {
    //   while (query.next()) {
    //     std::cout << query.value(0).toInt() << std::endl;
    //   }
    // }

  } catch (const std::exception& exception) {
    std::cerr << "\033[0;31mError\033[0m: " << exception.what() << std::endl;
    _errored = true;
  }
}

int Application::exec() { return _errored ? EXIT_FAILURE : _handle->exec(); }