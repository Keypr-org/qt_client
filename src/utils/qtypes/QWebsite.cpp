#include "QWebsite.h"
#include "entities/Website.h"

QWebsite::QWebsite(Entry *website) : QEntry(website)
{
    if (dynamic_cast<Website *>(website) == nullptr)
    {
        throw std::invalid_argument("QWebsite must be constructed with a Website entry.");
    }
}

const QString QWebsite::getTitle() const
{
    return QString::fromStdString(dynamic_cast<Website *>(entry)->getTitle());
}

void QWebsite::setTitle(const QString &title)
{
    dynamic_cast<Website *>(entry)->setTitle(title.toStdString());
}

const QString QWebsite::getComments() const
{
    return QString::fromStdString(dynamic_cast<Website *>(entry)->getComments());
}

void QWebsite::setComments(const QString &comments)
{
    dynamic_cast<Website *>(entry)->setComments(comments.toStdString());
}

const QString QWebsite::getUsername() const
{
    return QString::fromStdString(dynamic_cast<Website *>(entry)->getUsername());
}

void QWebsite::setUsername(const QString &username)
{
    dynamic_cast<Website *>(entry)->setUsername(username.toStdString());
}

const QString QWebsite::getPassword() const
{
    return QString::fromStdString(dynamic_cast<Website *>(entry)->getPassword());
}

void QWebsite::setPassword(const QString &password)
{
    dynamic_cast<Website *>(entry)->setPassword(password.toStdString());
}

const QString QWebsite::getUrl() const
{
    return QString::fromStdString(dynamic_cast<Website *>(entry)->getUrl());
}

void QWebsite::setUrl(const QString &url)
{
    dynamic_cast<Website *>(entry)->setUrl(url.toStdString());
}

const qint64 QWebsite::getPersonaId() const
{
    return dynamic_cast<Website *>(entry)->getPersonaId();
}

void QWebsite::setPersonaId(const qint64 personaId)
{
    dynamic_cast<Website *>(entry)->setPersona(personaId);
}

const QString QWebsite::getAliasId() const
{
    return QString::fromStdString(dynamic_cast<Website *>(entry)->getAliasId());
}

void QWebsite::setAliasId(const QString &aliasId)
{
    dynamic_cast<Website *>(entry)->setAliasId(aliasId.toStdString());
}

const QString QWebsite::getAlias() const
{
    return QString::fromStdString(dynamic_cast<Website *>(entry)->getAlias());
}

void QWebsite::setAlias(const QString &alias)
{
    dynamic_cast<Website *>(entry)->setAlias(alias.toStdString());
}

QEntry::EntryKind QWebsite::getKind() const
{
    return EntryKind::Website;
}