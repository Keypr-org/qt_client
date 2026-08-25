#include "clickablewidget.h"
#include <QMouseEvent>

ClickableWidget::ClickableWidget(QWidget *parent)
    : QWidget(parent)
{
    setCursor(Qt::PointingHandCursor);
}

void ClickableWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();

    QWidget::mousePressEvent(event);
}