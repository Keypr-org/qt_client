#include "toggleswitch.h"
#include <QPainter>
#include <QMouseEvent>

ToggleSwitch::ToggleSwitch(QWidget *parent)
    : QWidget(parent)
{
    setCursor(Qt::PointingHandCursor);
    m_animation = new QPropertyAnimation(this, "handlePosition", this);
    m_animation->setDuration(150);
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);
}

void ToggleSwitch::setChecked(bool checked)
{
    if (m_checked == checked)
        return;

    m_checked = checked;

    int endPos = m_checked ? (width() - height() + 3) : 3;
    m_animation->stop();
    m_animation->setStartValue(m_handlePosition);
    m_animation->setEndValue(endPos);
    m_animation->start();

    emit toggled(m_checked);
}

void ToggleSwitch::setHandlePosition(int pos)
{
    m_handlePosition = pos;
    update();
}

void ToggleSwitch::mousePressEvent(QMouseEvent *event)
{
    if (!isEnabled())
        return;
    Q_UNUSED(event);
    setChecked(!m_checked);
}

void ToggleSwitch::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    m_hovered = true;
    update();
}

void ToggleSwitch::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_hovered = false;
    update();
}

void ToggleSwitch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int h = height();
    int w = width();

    QColor bgColor = m_checked ? m_accentColor : m_trackColor;

    if (!isEnabled()) {
        bgColor = bgColor.darker(150);
    } else if (m_hovered) {
        bgColor = bgColor.lighter(115);
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawRoundedRect(0, 0, w, h, h / 2, h / 2);

    int diameter = h - 6;
    QColor handleColor = isEnabled() ? Qt::white : QColor("#9CA3AF");

    painter.setBrush(QColor(0, 0, 0, 40));
    painter.drawEllipse(m_handlePosition, 4, diameter, diameter);

    painter.setBrush(handleColor);
    painter.drawEllipse(m_handlePosition, 3, diameter, diameter);
}