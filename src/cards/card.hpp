#include "pages/page.hpp"

#include <functional>

class Card : public QWidget
{
    Q_OBJECT
private:
    const QStackedWidget* _stack;
    Page* _page;
public:
    Card(QWidget* parent, const QStackedWidget* stack, Page* page);

    inline const Page* page() { return _page; }
signals:
    void clicked(int);
protected:
    void mousePressEvent(QMouseEvent* event) override;
};
