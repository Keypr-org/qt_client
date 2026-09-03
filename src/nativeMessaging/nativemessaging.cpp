#include "nativemessaging.h"

#include <QThread>
#include <QMutexLocker>

#include <array>
#include <cstdint>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

#ifdef Q_OS_WIN
#include <io.h>
#else
#include <poll.h>
#include <unistd.h>
#endif

NativeMessaging::NativeMessaging(QObject *parent)
    : QObject{parent}
{
}

NativeMessaging::~NativeMessaging()
{
    stop();
}

void NativeMessaging::start()
{
    m_stopRequested = false;
    m_thread = QThread::create([this]()
                               { readMessages(); });

    m_thread->setParent(this);

    m_thread->start();
}

void NativeMessaging::stop()
{
    if (m_thread == nullptr)
    {
        return;
    }

    QThread *thread = m_thread;
    m_thread = nullptr;

    if (thread->isRunning())
    {
        m_stopRequested = true;
        closeStandardInput();
        thread->wait();
    }

    delete thread;
}

void NativeMessaging::closeStandardInput()
{
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

void NativeMessaging::readMessages()
{
    while (true)
    {
        // Native Messaging uses a 4-byte unsigned integer
        // containing the size of the JSON message.
        std::uint32_t messageLength = 0;

#ifdef Q_OS_WIN
        std::cin.read(reinterpret_cast<char *>(&messageLength), sizeof(messageLength));
        if (!std::cin || m_stopRequested)
        {
            emit finished();
            return;
        }
#else
        std::array<char, sizeof(messageLength)> lengthBytes;
        for (char &byte : lengthBytes)
        {
            while (!m_stopRequested)
            {
                pollfd input = {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0};
                const int pollResult = poll(&input, 1, 100);
                if (pollResult < 0 && errno == EINTR)
                {
                    continue;
                }
                if (pollResult < 0)
                {
                    emit finished();
                    return;
                }
                if (pollResult == 0)
                {
                    continue;
                }
                const ssize_t bytesRead = read(STDIN_FILENO, &byte, 1);
                if (bytesRead == 1)
                {
                    break;
                }
                if (bytesRead < 0 && errno == EINTR)
                {
                    continue;
                }
                emit finished();
                return;
            }
            if (m_stopRequested)
            {
                emit finished();
                return;
            }
        }
        std::memcpy(&messageLength, lengthBytes.data(), sizeof(messageLength));
#endif

        // Prevent absurdly large messages.
        constexpr std::uint32_t maxMessageSize = 1024 * 1024;

        if (messageLength > maxMessageSize)
        {
            emit errorOccurred("Native message is too large");
            emit finished();
            return;
        }

#ifdef Q_OS_WIN
        std::string message(messageLength, '\0');
        std::cin.read(message.data(), static_cast<std::streamsize>(messageLength));
        if (!std::cin || m_stopRequested)
        {
            emit finished();
            return;
        }
#else
        std::string message(messageLength, '\0');
        for (char &byte : message)
        {
            while (!m_stopRequested)
            {
                pollfd input = {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0};
                const int pollResult = poll(&input, 1, 100);
                if (pollResult < 0 && errno == EINTR)
                {
                    continue;
                }
                if (pollResult < 0)
                {
                    emit finished();
                    return;
                }
                if (pollResult == 0)
                {
                    continue;
                }
                const ssize_t bytesRead = read(STDIN_FILENO, &byte, 1);
                if (bytesRead == 1)
                {
                    break;
                }
                if (bytesRead < 0 && errno == EINTR)
                {
                    continue;
                }
                emit finished();
                return;
            }
            if (m_stopRequested)
            {
                emit finished();
                return;
            }
        }
#endif

        emit messageReceived(
            QByteArray::fromStdString(message));
    }
}

void NativeMessaging::sendMessage(const QByteArray &message)
{
    QMutexLocker locker(&m_writeMutex);

    const std::uint32_t messageLength = static_cast<std::uint32_t>(message.size());

    std::cout.write(
        reinterpret_cast<const char *>(&messageLength),
        sizeof(messageLength));

    std::cout.write(
        message.constData(),
        message.size());

    std::cout.flush();
}
