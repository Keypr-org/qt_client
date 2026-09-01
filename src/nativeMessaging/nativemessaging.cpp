#include "nativemessaging.h"

#include <QDebug>
#include <QThread>
#include <QMutexLocker>

#include <cstdint>
#include <iostream>
#include <string>

NativeMessaging::NativeMessaging(QObject *parent)
    : QObject{parent}
{}

void NativeMessaging::start() {
    QThread *thread = QThread::create([this]() {
        readMessages();
    });

    thread->setParent(this);

    connect(thread, &QThread::finished,
            thread, &QObject::deleteLater);

    thread->start();
}

void NativeMessaging::readMessages() {
    while (true) {
        // Native Messaging uses a 4-byte unsigned integer
        // containing the size of the JSON message.
        std::uint32_t messageLength = 0;

        std::cin.read(
            reinterpret_cast<char *>(&messageLength),
            sizeof(messageLength)
            );

        if (!std::cin) {
            return;
        }

        // Prevent absurdly large messages.
        constexpr std::uint32_t maxMessageSize = 1024 * 1024;

        if (messageLength > maxMessageSize) {
            emit errorOccurred("Native message is too large");
            return;
        }

        std::string message(messageLength, '\0');

        std::cin.read(
            message.data(),
            static_cast<std::streamsize>(messageLength)
            );

        if (!std::cin) {
            return;
        }

        emit messageReceived(
            QByteArray::fromStdString(message)
            );
    }
}

void NativeMessaging::sendMessage(const QByteArray &message) {
    QMutexLocker locker(&m_writeMutex);

    const std::uint32_t messageLength = static_cast<std::uint32_t>(message.size());

    std::cout.write(
        reinterpret_cast<const char *>(&messageLength),
        sizeof(messageLength)
        );

    std::cout.write(
        message.constData(),
        message.size()
        );

    std::cout.flush();
}