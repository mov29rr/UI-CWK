#pragma once

#include "page.hpp"

#include "charts/pollutantContaminationGraph.hpp"

/**
 * The PCB determinand structure.
 * 
 * Identifies a PCB in the data.
 */
struct PcbDeterminand
{
    /// The PCB ID
    int id;
    /// The PCB name
    QString name;
    /// The PCB concentration units
    QString units;
};

struct PcbMeasurement
{
    QDateTime date;
    qreal concentration;
};

/**
 * The persistent organic pollutants page class.
 */
class PersistentOrganicPollutantsPage : public Page
{
private:
    std::vector<PcbDeterminand> _pcbIdentifiers;
public:
    /**
     * Constructs the persistent organic pollutants page.
     */
    PersistentOrganicPollutantsPage();
};