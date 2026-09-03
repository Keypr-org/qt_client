#include "wifientry.h"
#include "ui_wifientry.h"

#include "component/notificationtooltip.h"
#include "../../utils/qtypes/QWifi.h"

WifiEntry::WifiEntry(QWidget *parent)
    : QWidget(parent), ui(new Ui::WifiEntry)
{
    ui->setupUi(this);

    ui->nameInput->setLabelText("network name (ssid)");
    ui->passwordInput->setLabelText("password");
    ui->notesInput->setLabelText("notes (optional");
    ui->notesInput->setInputPlaceholder("Enter notes here...");

    connect(ui->deleteButton, &QPushButton::clicked, this, [this]()
            { emit deleteRequested(m_entryId); });

    connect(ui->applyButton, &QPushButton::clicked, this, [this]()
            {

        if (ui->nameInput->text().isEmpty() || ui->passwordInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a network name and password.");
            return;
        }

        emit entrySaveRequested(m_entryId, ui->nameInput->text(), ui->passwordInput->text(), ui->notesInput->text()); });
}

WifiEntry::~WifiEntry()
{
    delete ui;
}

void WifiEntry::setEntry(const QEntry &entry)
{
    if (entry.getKind() != QEntry::EntryKind::Wifi)
    {
        return;
    }
    auto *wifiEntry = dynamic_cast<const QWifi *>(&entry);
    m_entryId = wifiEntry->getId();

    ui->titleLabel->setText(wifiEntry->getNetworkName());
    ui->nameInput->setText(wifiEntry->getNetworkName());
    ui->passwordInput->setText(wifiEntry->getPassword());
    ui->notesInput->setText(wifiEntry->getNotes());
}
