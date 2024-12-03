#pragma once

#include "page.hpp"

/**
 * The pollutant overview page
 */
class PollutantOverviewPage : public Page
{
public:
    /**
     * Constructs the pollutant overview page
     */
    PollutantOverviewPage() : Page("Pollutant Overview Page")
    {
        attachLayout();
    }
};