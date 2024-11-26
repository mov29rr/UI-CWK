#include "window.hpp"

/**
 * The dashboard class
 */
class Dashboard : public Window
{
private:
    /// Temporary example page contents
    QLabel exampleContent;
public:
    Dashboard()
        : exampleContent("Page contents")
    {
        content.addWidget(&exampleContent);

        finalise();
    }
};