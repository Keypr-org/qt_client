#include "QWifi.h"
#include "entities/Wifi.h"

QWifi::QWifi(Entry *wifi) : QEntry(wifi)
{
    if (dynamic_cast<Wifi *>(wifi) == nullptr)
    {
        throw std::invalid_argument("QWifi must be constructed with a Wifi entry.");
    }
}

const QString QWifi::getNetworkName() const
{
    return QString::fromStdString(dynamic_cast<Wifi *>(entry)->getNetworkName());
}

void QWifi::setNetworkName(const QString &networkName)
{
    dynamic_cast<Wifi *>(entry)->setNetworkName(networkName.toStdString());
}

const QString QWifi::getPassword() const
{
    return QString::fromStdString(dynamic_cast<Wifi *>(entry)->getPassword());
}

void QWifi::setPassword(const QString &password)
{
    dynamic_cast<Wifi *>(entry)->setPassword(password.toStdString());
}

QEntry::EntryKind QWifi::getKind() const
{
    return EntryKind::Wifi;
}