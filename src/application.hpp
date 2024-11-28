#include "window.hpp"

#include <memory>

class Application
{
private:
    QApplication _handle;

    std::unique_ptr<Window> _window;
public:
    Application(int argc, char** argv)
        : _handle(argc, argv)
        , _window(std::make_unique<Window>())
    {
        _window->show();
    }

    int exec()
    {
        return _handle.exec();
    }
};