#pragma once

#include "pages/page.hpp"

class MainDashboardPage : public Page
{
public:
    MainDashboardPage(Window* window, QListWidget* pageList, QStackedWidget* pageStack, const std::vector<Page*>& pages);
};