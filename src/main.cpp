#include "mainwindow.h"

#include <QApplication>

#include "nativeMessaging/nativemessaging.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.showMaximized();

    NativeMessaging nativeMessaging;

    QObject::connect(
        &nativeMessaging,
        &NativeMessaging::messageReceived,
        [&nativeMessaging](const QByteArray &message) {
            qDebug() << "Received:" << message;

            nativeMessaging.sendMessage(
                R"({"type":"ERROR","code":"TEST_RESPONSE"})"
                );
        }
        );

    QObject::connect(
        &nativeMessaging,
        &NativeMessaging::errorOccurred,
        [](const QString &error) {
            qWarning() << "Native Messaging error:" << error;
        }
        );

    nativeMessaging.start();

    return QApplication::exec();
}
