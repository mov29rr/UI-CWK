#include <QtWidgets>

class Page : public QWidget
{
public:
    virtual const char* title() = 0;
};