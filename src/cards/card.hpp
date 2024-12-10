#include "pages/page.hpp"

/**
 * The card class.
 */
class Card : public QWidget
{
    Q_OBJECT
private:
    const QStackedWidget* _stack;
    Page* _page;
public:
    /**
     * Constructs a card.
     * 
     * @param parent the parent widget.
     * @param stack the widget stack
     */
    Card(QWidget* parent, const QStackedWidget* stack, Page* page);
signals:
    /**
     * @brief The clicked signal.
     * 
     * @param index the card index.
     */
    void clicked(int index);
protected:
    void mousePressEvent(QMouseEvent* event) override;
};
