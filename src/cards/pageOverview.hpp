#include <QLabel>
#include <QMouseEvent>

#include "pages/page.hpp"

/**
 * The card class.
 */
class PageOverviewCard : public QWidget {
  Q_OBJECT
 private:
  const QStackedWidget* _stack;
  Page* _page;

 public:
  /**
   * Constructs the page overview cardcard.
   *
   * @param parent the parent widget.
   * @param stack the widget stack
   */
  PageOverviewCard(QWidget* parent, const QStackedWidget* stack, Page* page);
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
