#include "dashboard.hpp"

#include <memory>

class Application
{
private:
    QApplication _handle;

    std::unique_ptr<Dashboard> _dashboard;
public:
    Application(int argc, char** argv)
        : _handle(argc, argv)
        , _dashboard(std::make_unique<Dashboard>())
    {
        _dashboard->show();
    }

    int exec()
    {
        return _handle.exec();
    }
};