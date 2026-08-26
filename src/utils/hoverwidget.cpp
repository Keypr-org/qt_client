#include "hoverwidget.h"

#include <QEnterEvent>
#include <QEvent>
#include <QStyle>

HoverWidget::HoverWidget(QWidget *parent)
    : ClickableWidget(parent)
{
    setProperty("hovered", false);
}

void HoverWidget::enterEvent(QEnterEvent *event)
{
    setHovered(true);
    emit hoverEntered();

    ClickableWidget::enterEvent(event);
}

void HoverWidget::leaveEvent(QEvent *event)
{
    setHovered(false);
    emit hoverLeft();

    ClickableWidget::leaveEvent(event);
}

void HoverWidget::setHovered(bool hovered)
{
    if (property("hovered").toBool() == hovered) {
        return;
    }

    setProperty("hovered", hovered);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

