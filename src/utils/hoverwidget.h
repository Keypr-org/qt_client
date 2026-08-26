#ifndef HOVERWIDGET_H
#define HOVERWIDGET_H

#include "clickablewidget.h"

class HoverWidget : public ClickableWidget
{
    Q_OBJECT
public:
    explicit HoverWidget(QWidget *parent = nullptr);

signals:
    void hoverEntered();
    void hoverLeft();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void setHovered(bool hovered);
};

#endif // HOVERWIDGET_H