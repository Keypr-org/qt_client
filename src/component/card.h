#ifndef CARD_H
#define CARD_H

#include <QWidget>

namespace Ui {
class Card;
}

class Card : public QWidget
{
    Q_OBJECT

public:
    explicit Card(
        QWidget *parent = nullptr);
    ~Card();

    void setCardNumber(const QString &cardNumber);
    void setOwnerName(const QString &ownerName);
    void setExpiration(const QString &expiration);
    void setVisibility(bool visibility);

private:
    Ui::Card *ui;


    QString cardNumber;
    bool isVisible;

    void updateCardNumberLabel();
};

#endif // CARD_H
