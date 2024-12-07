#pragma once

#include "page.hpp"

#include "charts/pollutantContaminationGraph.hpp"

#include <unordered_map>

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

/**
 * The persistent organic pollutants page class.
 */
class PersistentOrganicPollutantsPage : public Page
{
private:
    std::vector<PcbDeterminand> _pcbs;
    // TODO: Caching system
    std::unordered_map<PcbDeterminand*, std::unique_ptr<std::vector<PollutantContaminationGraph::Point>>> _pcbMeasurements;
public:
    /**
     * Constructs the persistent organic pollutants page.
     */
    PersistentOrganicPollutantsPage();
};