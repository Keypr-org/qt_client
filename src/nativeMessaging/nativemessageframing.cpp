#include "nativemessageframing.h"

#include <cstdint>
#include <cstring>

QByteArray frameNativeMessage(const QByteArray &payload)
{
    const std::uint32_t length = static_cast<std::uint32_t>(payload.size());

    QByteArray frame(reinterpret_cast<const char *>(&length), sizeof(length));
    frame.append(payload);

    return frame;
}

NativeMessageReassembler::NativeMessageReassembler(QObject *parent)
    : QObject{parent}
{}

void NativeMessageReassembler::addData(const QByteArray &chunk)
{
    m_buffer.append(chunk);

    constexpr qsizetype lengthPrefixSize = sizeof(std::uint32_t);
    constexpr std::uint32_t maxMessageSize = 1024 * 1024;

    while (true) {
        if (m_buffer.size() < lengthPrefixSize) {
            return;
        }

        std::uint32_t length = 0;
        std::memcpy(&length, m_buffer.constData(), lengthPrefixSize);

        if (length > maxMessageSize) {
            m_buffer.clear();
            return;
        }

        const qsizetype frameSize = lengthPrefixSize + static_cast<qsizetype>(length);

        if (m_buffer.size() < frameSize) {
            return;
        }

        const QByteArray payload = m_buffer.mid(lengthPrefixSize, length);
        m_buffer.remove(0, frameSize);

        emit messageReady(payload);
    }
}
