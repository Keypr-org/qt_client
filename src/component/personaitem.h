#ifndef PERSONAITEM_H
#define PERSONAITEM_H

#include <QWidget>

namespace Ui {
class PersonaItem;
}

class PersonaItem : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaItem(QWidget *parent = nullptr);
    ~PersonaItem();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::PersonaItem *ui;
};

#endif // PERSONAITEM_H
