#ifndef NATIVEMESSAGEFRAMING_H
#define NATIVEMESSAGEFRAMING_H

#include <QByteArray>
#include <QObject>

/**
 * @brief Encodes a payload with the same 4-byte little-endian length prefix used by Chrome's Native Messaging protocol.
 * @param payload The raw message payload to frame.
 * @return The length-prefixed frame, ready to write to a stream.
 */
QByteArray frameNativeMessage(const QByteArray &payload);

/**
 * @class NativeMessageReassembler
 * @brief Accumulates raw bytes from a stream (QLocalSocket, ...) and emits each complete length-prefixed message as it becomes available.
 */
class NativeMessageReassembler : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a NativeMessageReassembler object.
     * @param parent A pointer to the parent QObject, default is nullptr.
     */
    explicit NativeMessageReassembler(QObject *parent = nullptr);

    /**
     * @brief Feeds newly received bytes into the reassembler, emitting messageReady() for each complete frame found.
     * @param chunk The raw bytes just read from the stream.
     */
    void addData(const QByteArray &chunk);

signals:
    /**
     * @brief Emitted once a complete length-prefixed message has been reassembled.
     * @param payload The reassembled message payload, with the length prefix stripped.
     */
    void messageReady(const QByteArray &payload);

private:
    QByteArray m_buffer;
};

#endif // NATIVEMESSAGEFRAMING_H
