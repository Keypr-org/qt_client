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
    const PersonaData &persona() const;

signals:
    void modifyRequested(PersonaData persona);
    void deleteRequested(QString id);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::PersonaItem *ui;
    PersonaData m_persona;
};

#endif // PERSONAITEM_H
