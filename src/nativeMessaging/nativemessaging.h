#ifndef NATIVEMESSAGING_H
#define NATIVEMESSAGING_H

#include <QObject>
#include <QMutex>

#include <atomic>
#include <cstdint>

class QThread;

class NativeMessaging : public QObject
{
    Q_OBJECT
public:
    explicit NativeMessaging(QObject *parent = nullptr);
    ~NativeMessaging() override;

    void start();

    // Requests the read loop to stop and waits for its worker thread to finish.
    // The POSIX reader polls stdin so this does not depend on Chrome closing
    // the native-messaging pipe.
    void stop();

public slots:
    void sendMessage(const QByteArray &message);

signals:
    void messageReceived(const QByteArray &message);
    void errorOccurred(const QString &error);

    // Emitted when the read loop ends on its own, e.g. because Chrome closed
    // the Native Messaging pipe (as opposed to being asked to stop()).
    void finished();

private:
    void readMessages();
    static void closeStandardInput();

    QThread *m_thread = nullptr;
    QMutex m_writeMutex;
    std::atomic_bool m_stopRequested = false;
};

#endif // NATIVEMESSAGING_H
