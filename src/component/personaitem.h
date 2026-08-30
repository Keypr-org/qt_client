#ifndef PERSONAITEM_H
#define PERSONAITEM_H

#include <QWidget>

#include "model/persona.h"

namespace Ui {
class PersonaItem;
}

class PersonaItem : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaItem(QWidget *parent = nullptr);
    ~PersonaItem();

    void setPersona(const PersonaData &persona);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::PersonaItem *ui;
};

#endif // PERSONAITEM_H
