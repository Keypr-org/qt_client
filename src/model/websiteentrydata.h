#ifndef WEBSITEENTRYDATA_H
#define WEBSITEENTRYDATA_H

#include "entry.h"

struct WebsiteEntryData : public Entry
{
    QString username;
    QString password;
    QString url;
    QString description;
    QString notes;

    WebsiteEntryData(const QString &id, const QString &websiteName,
                      const QString &username, const QString &password,
                      const QString &url, const QString &description,
                      const QString &notes, const QDateTime &lastUpdated)
        : Entry(id, EntryType::Website, websiteName, username, lastUpdated)
        , username(username)
        , password(password)
        , url(url)
        , description(description)
        , notes(notes)
    {}
};

#endif // WEBSITEENTRYDATA_H
