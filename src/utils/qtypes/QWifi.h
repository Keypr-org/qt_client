#pragma once

#include "QEntry.h"

class QWifi : public QEntry
{
public:
    QWifi(Entry *wifi);

    const QString getNetworkName() const;
    void setNetworkName(const QString &networkName);

    const QString getPassword() const;
    void setPassword(const QString &password);

    EntryKind getKind() const override;
};