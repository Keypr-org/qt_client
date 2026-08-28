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
    explicit ToggleSwitch(QWidget *parent = nullptr);

    bool isChecked() const { return m_checked; }
    void setChecked(bool checked);

    int handlePosition() const { return m_handlePosition; }
    void setHandlePosition(int pos);

    QSize sizeHint() const override { return QSize(50, 26); }

    void setAccentColor(const QColor &color) { m_accentColor = color; update(); }
    void setTrackColor(const QColor &color) { m_trackColor = color; update(); }

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
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