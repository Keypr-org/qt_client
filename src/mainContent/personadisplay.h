#ifndef PERSONADISPLAY_H
#define PERSONADISPLAY_H

#include <QWidget>

namespace Ui {
class PersonaDisplay;
}

class PersonaDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaDisplay(QWidget *parent = nullptr);
    ~PersonaDisplay();

private:
    Ui::PersonaDisplay *ui;
};

#endif // PERSONADISPLAY_H
