#ifndef CLICKABLEWIDGET_H
#define CLICKABLEWIDGET_H

#include <QWidget>

class ClickableWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a widget that emits clicked() on mouse press.
     * @param parent Parent widget.
     */
    explicit ClickableWidget(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when the widget is clicked (pressed) with the mouse.
     */
    void clicked();

protected:
    /**
     * @brief Emits clicked() whenever a mouse press is received.
     * @param event Mouse press event data.
     */
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CLICKABLEWIDGET_H
