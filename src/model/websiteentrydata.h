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
    QString personaId; // empty when no persona is linked
    QString aliasEmail; // empty when no email alias has been generated
    QString aliasId; // Postscale alias identifier, empty when no email alias has been generated

    /**
     * @brief Constructs a fully-populated website entry with no persona linked.
     * @param id Unique entry identifier.
     * @param websiteName Display name of the website.
     * @param username Username or email used to log in.
     * @param password Account password.
     * @param url Website URL.
     * @param description Optional free-form description.
     * @param notes Optional free-form notes.
     * @param lastUpdated Timestamp of the last modification.
     */
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
