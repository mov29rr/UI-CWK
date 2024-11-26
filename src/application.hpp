#include "dashboard.hpp"

class Application
{
private:
    QApplication _handle;

    Dashboard _dashboard;
public:
    Application(int argc, char** argv)
        : _handle(argc, argv)
    {
        _dashboard.show();
    }

    int exec()
    {
        return _handle.exec();
    }
};