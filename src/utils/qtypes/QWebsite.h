#pragma once

#include "QEntry.h"

class QWebsite : public QEntry
{
public:
    QWebsite(Entry *website);

    const QString getTitle() const;
    void setTitle(const QString &title);

    const QString getComments() const;
    void setComments(const QString &comments);

    const QString getUsername() const;
    void setUsername(const QString &username);

    const QString getPassword() const;
    void setPassword(const QString &password);

    const QString getUrl() const;
    void setUrl(const QString &url);

    const qint64 getPersonaId() const;
    void setPersonaId(const qint64 personaId);

    const QString getAliasId() const;
    void setAliasId(const QString &aliasId);

    const QString getAlias() const;
    void setAlias(const QString &alias);

    EntryKind getKind() const override;
};