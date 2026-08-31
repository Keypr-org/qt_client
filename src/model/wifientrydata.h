#ifndef WIFIENTRYDATA_H
#define WIFIENTRYDATA_H

#include "entry.h"

struct WifiEntryData : public Entry
{
    QString ssid;
    QString password;
    QString notes;

    WifiEntryData(const QString &id, const QString &ssid,
                   const QString &password, const QString &notes,
                   const QDateTime &lastUpdated)
        : Entry(id, EntryType::Wifi, ssid, QString(), lastUpdated)
        , ssid(ssid)
        , password(password)
        , notes(notes)
    {}
};

#endif // WIFIENTRYDATA_H
