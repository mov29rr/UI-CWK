#pragma once

#include "charts/pollutantContaminationGraphBase.hpp"

class ComplianceDial : public QWidget {
  Q_OBJECT

 private:
  qreal& _value;
  Range<qreal> _range;
  ComplianceLevels _complianceLevels;

  const int _width = 250, _height = 125;

 public:
  ComplianceDial(qreal& value, Range<qreal> range, ComplianceLevels complianceLevels)
      : _value(value), _range(range), _complianceLevels(complianceLevels) {
    setFixedSize(_width, _height + 50);
    setWindowTitle("Speed Dial Indicator");
  }

 protected:
  void paintEvent(QPaintEvent*) override {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Drawing the container
    const qreal delta = _range.max - _range.min;
    QConicalGradient gradient(_width / 2, _height, 0);
    gradient.setColorAt(qBound(0.0, (1.0 - _complianceLevels.veryLow / delta) / 2, 1.0), 0x00FF00);
    gradient.setColorAt(qBound(0.0, (1.0 - mean(_complianceLevels.low, _complianceLevels.high) / delta) / 2, 1.0),
                        0xFFFF00);
    gradient.setColorAt(qBound(0.0, (1.0 - _complianceLevels.veryHigh / delta), 1.0) / 2, 0xFF0000);
    painter.setBrush(QBrush(gradient));
    painter.drawPie(0, 0, _width, _height * 2, 0, 180 * 16);

    // Drawing the needle
    painter.setPen(QPen(Qt::blue, 4));
    int needleLength = _width / 2 - 30;
    qreal angle = (_value / delta) * 180.0 + 180.0;
    qreal radian = angle * M_PI / 180.0;

    int needleX = _width / 2 + needleLength * qCos(radian);
    int needleY = _height - needleLength * qSin(radian);

    painter.drawLine(_width / 2, _height, needleX, needleY);

    // Drawing the text displaying the average concentration
    painter.setPen(QPen(Qt::black));
    painter.drawText(0, _height + 25, "Average concentration: " + QString::number(_value));
  }
};
