#include "entrieitem.h"
#include "ui_entrieitem.h"
#include <QPixmap>

const QString WEBSITE_ICON_WHITE = ":/icons/icons/icon-globe.png";
const QString WEBSITE_ICON_COLOR = ":/icons/icons/icon-globe-purple.png";

const QString WIFI_ICON_WHITE = ":/icons/icons/icon-wifi.png";
const QString WIFI_ICON_COLOR = ":/icons/icons/icon-wifi-purple.png";

const QString CREDIT_CARD_ICON_WHITE = ":/icons/icons/icon-credit-card.png";
const QString CREDIT_CARD_ICON_COLOR = ":/icons/icons/icon-credit-card-purple.png";

EntrieItem::EntrieItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EntrieItem)
{
    ui->setupUi(this);
}

EntrieItem::~EntrieItem()
{
    delete ui;
}

void EntrieItem::setIcon(IconType type)
{
    currentIcon = type;

    setSelected(false);
}

void EntrieItem::setSelected(bool selected)
{
    QString iconPath;

    if (selected)
    {
        switch (currentIcon)
        {
        case IconType::WebSite:
            iconPath = WEBSITE_ICON_COLOR;
            break;

        case IconType::Wifi:
            iconPath = WIFI_ICON_COLOR;
            break;

        case IconType::CreditCard:
            iconPath = CREDIT_CARD_ICON_COLOR;
            break;
        }

        this->setStyleSheet(
            "background: #1F2937;"
            "border-bottom: 1px solid #A91EE4;"
            );
        ui->entrieIcon->setStyleSheet(
            "background-color: #2F274A;"
            "border: 1px solid #A91EE4;"
            "border-radius: 6px;"
            );
        ui->secondaryInfo->setStyleSheet(
            "color: #A91EE4;"
            "border: none;"
            );
    }
    else
    {
        switch (currentIcon)
        {
        case IconType::WebSite:
            iconPath = WEBSITE_ICON_WHITE;
            break;

        case IconType::Wifi:
            iconPath = WIFI_ICON_WHITE;
            break;

        case IconType::CreditCard:
            iconPath = CREDIT_CARD_ICON_WHITE;
            break;
        }

        this->setStyleSheet(
            "background: transparent;"
            "border-bottom: 1px solid #1F2937;"
            );
        ui->entrieIcon->setStyleSheet(
            "background-color: #0F0F0A;"
            "border: 1px solid #1F2937;"
            "border-radius: 6px;"
            );
        ui->secondaryInfo->setStyleSheet(
            "color: #9CA3AF;"
            );
    }

    ui->entrieIcon->setPixmap(QPixmap(iconPath));
}

void EntrieItem::setPrimaryInfo(const QString &text) {
    ui->primaryInfo->setText(text);
}
void EntrieItem::setSecondaryInfo(const QString &text) {
    ui->secondaryInfo->setText(text);
}

void EntrieItem::setLastUpdate(const QString &text) {
    ui->lastUpdateLabel->setText(text + "ago");
}