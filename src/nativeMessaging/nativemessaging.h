#ifndef NATIVEMESSAGING_H
#define NATIVEMESSAGING_H

#include <QObject>
#include <QMutex>

#include <cstdint>

class NativeMessaging : public QObject
{
    Q_OBJECT
public:
    explicit NativeMessaging(QObject *parent = nullptr);

    void start();

public slots:
    void sendMessage(const QByteArray &message);

signals:
    void messageReceived(const QByteArray &message);
    void errorOccurred(const QString &error);

private:
    void readMessages();

    QMutex m_writeMutex;
};

#endif // NATIVEMESSAGING_H
