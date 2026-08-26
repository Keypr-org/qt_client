#ifndef ENTRIEITEM_H
#define ENTRIEITEM_H

#include <QWidget>

namespace Ui {
class EntrieItem;
}

class EntrieItem : public QWidget
{
    Q_OBJECT

public:
    enum IconType {
        WebSite,
        Wifi,
        CreditCard
    };


    explicit EntrieItem(QWidget *parent = nullptr);
    ~EntrieItem();

    void setIcon(IconType type);
    void setSelected(bool selected);
    void setPrimaryInfo(const QString &text);
    void setSecondaryInfo(const QString &text);
    void setLastUpdate(const QString &text);

private:
    Ui::EntrieItem *ui;
    IconType currentIcon;
};

#endif // ENTRIEITEM_H
