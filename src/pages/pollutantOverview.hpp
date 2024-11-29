#pragma once

#include "page.hpp"

/**
 * The pollutant overview page
 */
class PollutantOverviewPage : public Page
{
public:
    PollutantOverviewPage() : Page("Pollutant Overview Page")
    {
        attachLayout();
    }
};