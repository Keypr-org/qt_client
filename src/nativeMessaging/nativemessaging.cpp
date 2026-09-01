#include "nativemessaging.h"

#include <QDebug>
#include <QThread>
#include <QMutexLocker>

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>

#ifdef Q_OS_WIN
#include <io.h>
#else
#include <unistd.h>
#endif

NativeMessaging::NativeMessaging(QObject *parent)
    : QObject{parent}
{}

NativeMessaging::~NativeMessaging()
{
    stop();
}

void NativeMessaging::start() {
    m_thread = QThread::create([this]() {
        readMessages();
    });

    m_thread->setParent(this);

    connect(m_thread, &QThread::finished,
            m_thread, &QObject::deleteLater);

    m_thread->start();
}

void NativeMessaging::stop() {
    if (m_thread == nullptr) {
        return;
    }

    if (m_thread->isRunning()) {
        // Best-effort: closing the descriptor unblocks the read loop on some
        // platforms/setups, but POSIX leaves the effect of closing an fd
        // while another thread is blocked reading it unspecified, so it
        // isn't reliable everywhere. Don't wait forever for Chrome to close
        // its end of the pipe either way: bound the wait so quitting stays
        // fast regardless. If the thread is still blocked afterwards, it
        // gets reclaimed when the process exits.
        closeStandardInput();
        m_thread->wait(500);
    }

    m_thread = nullptr;
}

void NativeMessaging::closeStandardInput() {
    // Deliberately closes the raw descriptor (0) rather than going through
    // fileno(stdin)/_fileno(stdin): those lock the FILE stream, and the
    // worker thread holds that lock for the entire duration of its blocking
    // read, which would make this call block right along with it.
#ifdef Q_OS_WIN
    _close(0);
#else
    close(STDIN_FILENO);
#endif
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
            emit finished();
            return;
        }

        // Prevent absurdly large messages.
        constexpr std::uint32_t maxMessageSize = 1024 * 1024;

        if (messageLength > maxMessageSize) {
            emit errorOccurred("Native message is too large");
            emit finished();
            return;
        }

        std::string message(messageLength, '\0');

        std::cin.read(
            message.data(),
            static_cast<std::streamsize>(messageLength)
            );

        if (!std::cin) {
            emit finished();
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
