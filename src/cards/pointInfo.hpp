#pragma once

#include "core/pollutantContamination.hpp"

#include <QtWidgets>

class PointInfoCard : public QWidget
{
    Q_OBJECT
public:
    PointInfoCard(PollutantContaminationPoint* point = nullptr)
    {
        setFixedSize(200, 200);

        auto layout = new QVBoxLayout;

        auto card = new QWidget;
        auto cardLayout = new QVBoxLayout;

        card->setObjectName("card");

        if (!point)
        {
            auto text = new QLabel(tr("Select a point to display information"));
            text->setWordWrap(true);
            cardLayout->addWidget(text);
        }
        else
        {
            cardLayout->addStretch();
            cardLayout->addWidget(new QLabel("TODO: Pollutant name"));
            cardLayout->addWidget(new QLabel("Time: " + point->time.toString()));
            cardLayout->addWidget(new QLabel("Concentration: " + QString::number(point->concentration)));
            cardLayout->addStretch();
        }

        cardLayout->setAlignment(Qt::AlignCenter);

        card->setLayout(cardLayout);
        layout->addWidget(card);

        setLayout(layout);
    }
};