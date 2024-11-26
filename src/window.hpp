#include "navBar.hpp"

/**
 * The window class
 */
class Window : private QWidget
{
private:
    /// The window title
    static constexpr const char* TITLE = "Water Quality Inspector";
    static constexpr int
        WIDTH = 1200, ///< The window width
        HEIGHT = 600; ///< The window height

    /// The window navigation bar
    NavBar _navBar;
    /// The window page layout
    QHBoxLayout _page;
protected:
    /// The window content, which derrived instances add to
    QVBoxLayout content;
public:
    /**
     * Constructs a window
     */
    Window()
    {
        setWindowTitle(TITLE);

        content.setContentsMargins(0, 0, WIDTH, HEIGHT);

        _page.addWidget(&_navBar);
        _page.addLayout(&content);
    }

    /**
     * Finalises the window layout
     * 
     * @remark Should only be called when all widgets have been added to the content.
     */
    void finalise()
    {
        setLayout(&_page);
    }

    /**
     * Sets the window to be visible
     */
    void show()
    {
        QWidget::show();
    }
};