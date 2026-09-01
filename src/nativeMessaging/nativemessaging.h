#ifndef NATIVEMESSAGING_H
#define NATIVEMESSAGING_H

#include <QObject>
#include <QMutex>

#include <cstdint>

class QThread;

class NativeMessaging : public QObject
{
    Q_OBJECT
public:
    explicit NativeMessaging(QObject *parent = nullptr);
    ~NativeMessaging() override;

    void start();

    // Tries to unblock the read loop (parked in a blocking stdin read) and
    // waits briefly for it to finish. Called on app quit so shutdown doesn't
    // hang waiting for Chrome to close the pipe on its own; the wait is
    // bounded, so quitting stays fast even if the thread stays blocked.
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
};

#endif // NATIVEMESSAGING_H
