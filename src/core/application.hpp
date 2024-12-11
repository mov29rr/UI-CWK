#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "mainWindow.hpp"

/**
 * The application class
 */
class Application {
 private:
  static constexpr const char* STYLE_SHEET_FILE_PATH = "stylesheets/stylesheet.qss";

  std::unique_ptr<QApplication> _handle;
  std::unique_ptr<MainWindow> _window;

  bool _errored = false;

 public:
  /**
   * Constructs the application
   *
   * @param argc the argument count.
   * @param argv the argument vector.
   */
  Application(int argc, char** argv);

  /**
   * Executes the application
   */
  int exec();
};
// #include <QtWidgets>
// #include <QPainter>
// #include <QPen>
// #include <QBrush>
// #include <QTimer>
// #include <QTime>

// class SpeedDial : public QWidget
// {
//     Q_OBJECT

// public:
//     SpeedDial(QWidget *parent = nullptr) : QWidget(parent)
//     {
//         setFixedSize(300, 300);
//         setWindowTitle("Speed Dial Indicator");

//         // Timer to update the needle position every 100ms (for smooth animation)
//         timer = new QTimer(this);
//         connect(timer, &QTimer::timeout, this, &SpeedDial::updateNeedle);
//         timer->start(100);

//         // Initial needle value
//         currentValue = 0;
//     }

// protected:
//     void paintEvent(QPaintEvent *event) override
//     {
//         QPainter painter(this);
//         painter.setRenderHint(QPainter::Antialiasing);

//         // Draw the background circle (Speed Dial background)
//         painter.setBrush(QBrush(Qt::lightGray));
//         painter.drawEllipse(20, 20, width() - 40, height() - 40);

//         // Draw the scale (dial)
//         QPen pen;
//         pen.setColor(Qt::black);
//         pen.setWidth(3);
//         painter.setPen(pen);
//         painter.setBrush(Qt::NoBrush);
//         painter.drawArc(20, 20, width() - 40, height() - 40, 30 * 16, 300 * 16);

//         // Draw the value indicator (needle)
//         painter.setPen(QPen(Qt::red, 4));
//         int needleLength = width() / 2 - 30;
//         int angle = (currentValue / 100.0) * 240; // Scale the value to fit the dial (0-100 range)

//         // Convert angle to radians for trigonometric calculations
//         double radian = (angle - 120) * M_PI / 180.0;

//         // Needle coordinates
//         int needleX = width() / 2 + needleLength * qCos(radian);
//         int needleY = height() / 2 - needleLength * qSin(radian);

//         // Draw the needle
//         painter.drawLine(width() / 2, height() / 2, needleX, needleY);

//         // Draw value text in the center
//         painter.setPen(QPen(Qt::black));
//         painter.setFont(QFont("Arial", 16));
//         painter.drawText(rect(), Qt::AlignCenter, QString::number(currentValue) + " %");
//     }

// private slots:
//     void updateNeedle()
//     {
//         // Animate the needle by increasing the value
//         currentValue = (currentValue + 1) % 101; // Loop back after 100%
//         update(); // Repaint the widget
//     }

// private:
//     QTimer *timer;
//     int currentValue;
// };

// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);

//     SpeedDial dial;
//     dial.show();

//     return app.exec();
// }
