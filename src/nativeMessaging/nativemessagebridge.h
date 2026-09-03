#ifndef NATIVEMESSAGEBRIDGE_H
#define NATIVEMESSAGEBRIDGE_H

#include <QObject>

class QLocalSocket;
class NativeMessaging;
class NativeMessageReassembler;

/**
 * @class NativeMessageBridge
 * @brief Relays Native Messaging payloads between Chrome and the primary instance.
 */
class NativeMessageBridge : public QObject
{
    Q_OBJECT
public:
    /**
    * @brief Constructs a NativeMessageBridge object.
    * @param socket A pointer to a QLocalSocket object that is already connected to the primary instance.
    * @param nativeMessaging A pointer to a NativeMessaging object for handling native messaging.
    * @param parent A pointer to the parent QObject, default is nullptr.    
    */
    NativeMessageBridge(QLocalSocket *socket, NativeMessaging *nativeMessaging, QObject *parent = nullptr);

signals:
    /**
    * @brief Signal emitted when the primary instance disconnects.
    */
    void primaryDisconnected();

private:
    QLocalSocket *m_socket;
    NativeMessaging *m_nativeMessaging;
    NativeMessageReassembler *m_reassembler;
};

#endif // NATIVEMESSAGEBRIDGE_H
