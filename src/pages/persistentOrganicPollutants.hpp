#pragma once

#include "page.hpp"

#include "charts/complianceColourCodedLineGraph.hpp"

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
    std::string name;
};

struct PcbMeasurement
{
    QDateTime dateTime;
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