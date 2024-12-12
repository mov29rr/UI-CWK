#pragma once

#include <QtWidgets>

#include "core/pollutantContamination.hpp"

class PointInfoCard : public QWidget {
  Q_OBJECT
 public:
  PointInfoCard(PollutantContaminationPoint* point = nullptr) {
    setFixedSize(200, 200);

    auto layout = new QVBoxLayout;

    auto card = new QWidget;
    auto cardLayout = new QVBoxLayout;

    card->setObjectName("card");

    if (!point) {
      auto text = new QLabel(tr("Select a point to display information"));
      text->setWordWrap(true);
      cardLayout->addWidget(text);
    } else {
      cardLayout->addStretch();
      cardLayout->addWidget(new QLabel(tr("TODO: Pollutant name")));
      cardLayout->addWidget(new QLabel(tr("Time: ") + point->time.toString()));
      cardLayout->addWidget(new QLabel(tr("Concentration: ") + QString::number(point->concentration)));
      cardLayout->addStretch();
    }

    cardLayout->setAlignment(Qt::AlignCenter);

    card->setLayout(cardLayout);
    layout->addWidget(card);

    setLayout(layout);
  }
};