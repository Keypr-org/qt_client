#ifndef HOVERWIDGET_H
#define HOVERWIDGET_H

#include "clickablewidget.h"

class HoverWidget : public ClickableWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a clickable widget that also tracks mouse hover state.
     * @param parent Parent widget.
     */
    explicit HoverWidget(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when the mouse cursor enters the widget.
     */
    void hoverEntered();

    /**
     * @brief Emitted when the mouse cursor leaves the widget.
     */
    void hoverLeft();

protected:
    /**
     * @brief Marks the widget as hovered and emits hoverEntered() when the cursor enters it.
     * @param event Enter event data.
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief Marks the widget as not hovered and emits hoverLeft() when the cursor leaves it.
     * @param event Leave event data.
     */
    void leaveEvent(QEvent *event) override;

private:
    /**
     * @brief Updates the widget's internal hover state.
     * @param hovered Whether the widget is currently hovered.
     */
    void setHovered(bool hovered);
};

#endif // HOVERWIDGET_H