#pragma once

#include "pages/page.hpp"

/**
 * The main dash board page class.
 */
class MainDashboardPage : public Page
{
public:
    /**
     * Constructs the main dashboard page.
     * 
     * @param window the parent window.
     * @param pageList the sidebar list of pages.
     * @param pageStack the stack of page contents.
     * @param pages the array of pages to redirect to.
     */
    MainDashboardPage(Window* window, QListWidget* pageList, QStackedWidget* pageStack, const std::vector<Page*>& pages);
};