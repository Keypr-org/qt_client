#ifndef TOGGLESWITCH_H
#define TOGGLESWITCH_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QColor>

class ToggleSwitch : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int handlePosition READ handlePosition WRITE setHandlePosition)

public:
    /**
     * @brief Constructs an animated on/off toggle switch.
     * @param parent Parent widget.
     */
    explicit ToggleSwitch(QWidget *parent = nullptr);

    /**
     * @brief Returns whether the switch is currently on.
     * @return True if checked (on).
     */
    bool isChecked() const { return m_checked; }
    /**
     * @brief Sets the switch's checked state, animating the handle to its new position.
     * @param checked True to turn the switch on.
     */
    void setChecked(bool checked);

    /**
     * @brief Returns the handle's current horizontal animation position, in pixels.
     * @return The handle position.
     */
    int handlePosition() const { return m_handlePosition; }
    /**
     * @brief Sets the handle's horizontal animation position; used by the Qt property animation.
     * @param pos The handle position, in pixels.
     */
    void setHandlePosition(int pos);

    /**
     * @brief Returns the widget's preferred size.
     * @return The preferred size.
     */
    QSize sizeHint() const override { return QSize(50, 26); }

    /**
     * @brief Sets the color used for the switch's "on" state.
     * @param color The accent color.
     */
    void setAccentColor(const QColor &color) { m_accentColor = color; update(); }
    /**
     * @brief Sets the color used for the switch's track background.
     * @param color The track color.
     */
    void setTrackColor(const QColor &color) { m_trackColor = color; update(); }

signals:
    /**
     * @brief Emitted whenever the switch's checked state changes.
     * @param checked The new checked state.
     */
    void toggled(bool checked);

protected:
    /**
     * @brief Draws the track and handle in their current animated state.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;
    /**
     * @brief Toggles the switch when clicked.
     * @param event The mouse press event.
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief Marks the switch as hovered, updating its visual feedback.
     * @param event The hover-enter event.
     */
    void enterEvent(QEnterEvent *event) override;
    /**
     * @brief Clears the hovered state when the pointer leaves the switch.
     * @param event The hover-leave event.
     */
    void leaveEvent(QEvent *event) override;

private:
    bool m_checked = false;
    bool m_hovered = false;
    int m_handlePosition = 3;
    QPropertyAnimation *m_animation;

    QColor m_accentColor  = QColor("#A91EE4");
    QColor m_trackColor   = QColor("#374151");
};

#endif // TOGGLESWITCH_H