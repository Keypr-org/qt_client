#ifndef NATIVEMESSAGESERVER_H
#define NATIVEMESSAGESERVER_H

#include <QObject>

class QLocalServer;
class NativeMessageDispatcher;

/**
 * @class NativeMessageServer
 * @brief Runs on the primary qt_client instance: accepts local IPC connections from secondary qt_client processes that Chrome spawns via Native Messaging, and answers their requests through the same NativeMessageDispatcher the primary instance itself uses, so every process shares one source of truth.
 */
class NativeMessageServer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a NativeMessageServer object.
     * @param dispatcher The dispatcher used to answer incoming requests.
     * @param parent A pointer to the parent QObject, default is nullptr.
     */
    explicit NativeMessageServer(NativeMessageDispatcher *dispatcher, QObject *parent = nullptr);

    /**
     * @brief The well-known local server name used to detect/reach the primary instance.
     * @return The server name.
     */
    static QString serverName();

    /**
     * @brief Attempts to become the primary instance's server.
     * @return True on success; false if another process already holds the name (the caller should fall back to bridge mode instead).
     */
    bool start();

private slots:
    /**
     * @brief Accepts pending connections from secondary qt_client processes and wires each one to the dispatcher.
     */
    void handleNewConnection();

private:
    QLocalServer *m_server;
    NativeMessageDispatcher *m_dispatcher;
};

#endif // NATIVEMESSAGESERVER_H
