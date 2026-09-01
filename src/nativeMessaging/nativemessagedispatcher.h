#ifndef NATIVEMESSAGEDISPATCHER_H
#define NATIVEMESSAGEDISPATCHER_H

#include <QByteArray>
#include <QObject>

class EntryRepository;
class QJsonObject;

/**
 * @class NativeMessageDispatcher
 * @brief Turns a raw Native Messaging JSON request from the browser extension into a JSON response, backed by an EntryRepository.
 */
class NativeMessageDispatcher : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a NativeMessageDispatcher object.
     * @param repository The entry repository to answer requests from.
     * @param parent A pointer to the parent QObject, default is nullptr.
     */
    explicit NativeMessageDispatcher(EntryRepository *repository, QObject *parent = nullptr);

    /**
     * @brief Dispatches a raw Native Messaging request to the matching handler.
     * @param request The raw JSON request payload.
     * @return The raw JSON response payload (ENTRIES, PASSWORD, or ERROR).
     */
    QByteArray dispatch(const QByteArray &request) const;

private:
    /**
     * @brief Handles a GET_ENTRIES request.
     * @param request The parsed JSON request object.
     * @return The raw JSON ENTRIES (or ERROR) response payload.
     */
    QByteArray handleGetEntries(const QJsonObject &request) const;

    /**
     * @brief Handles a GET_PASSWORD request.
     * @param request The parsed JSON request object.
     * @return The raw JSON PASSWORD (or ERROR) response payload.
     */
    QByteArray handleGetPassword(const QJsonObject &request) const;

    /**
     * @brief Builds a raw JSON ERROR response payload.
     * @param code The error code to report.
     * @return The raw JSON ERROR response payload.
     */
    static QByteArray errorResponse(const QString &code);

    EntryRepository *m_repository;
};

#endif // NATIVEMESSAGEDISPATCHER_H
