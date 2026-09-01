#include "mainwindow.h"

#include <QApplication>
#include <QLocalSocket>

#include "model/entryrepository.h"
#include "nativeMessaging/nativemessagebridge.h"
#include "nativeMessaging/nativemessagedispatcher.h"
#include "nativeMessaging/nativemessaging.h"
#include "nativeMessaging/nativemessageserver.h"

namespace {

constexpr int kInstanceProbeTimeoutMs = 200;

// True if another qt_client process is already answering as the primary
// instance. On success, socket stays connected and can be reused by the
// caller instead of reconnecting.
bool tryConnectToPrimaryInstance(QLocalSocket &socket)
{
    socket.connectToServer(NativeMessageServer::serverName());
    return socket.waitForConnected(kInstanceProbeTimeoutMs);
}

// Chrome spawns a brand-new qt_client process on every
// chrome.runtime.connectNative() call. When a primary instance already
// exists, this process only relays Native Messaging bytes between Chrome
// (via stdio) and the primary (via localSocket); it never shows a window.
void runAsBridge(QApplication &app, QLocalSocket *localSocket)
{
    auto *nativeMessaging = new NativeMessaging(&app);
    auto *bridge = new NativeMessageBridge(localSocket, nativeMessaging, &app);

    QObject::connect(bridge, &NativeMessageBridge::primaryDisconnected,
                      &app, &QApplication::quit);
    QObject::connect(nativeMessaging, &NativeMessaging::finished,
                      &app, &QApplication::quit);
    QObject::connect(&app, &QApplication::aboutToQuit,
                      nativeMessaging, &NativeMessaging::stop);

    nativeMessaging->start();
}

void runAsPrimary(QApplication &app)
{
    auto *w = new MainWindow();
    w->setWindowTitle("Keypr | Your local password manager");
    w->showMaximized();

    auto *nativeMessaging = new NativeMessaging(&app);
    auto *repository = new EntryRepository(&app);
    auto *dispatcher = new NativeMessageDispatcher(repository, &app);
    auto *server = new NativeMessageServer(dispatcher, &app);

    if (!server->start()) {
        qWarning() << "Could not start the Native Messaging instance server; "
                       "other qt_client processes won't be able to reuse this instance.";
    }

    QObject::connect(
        nativeMessaging,
        &NativeMessaging::messageReceived,
        nativeMessaging,
        [nativeMessaging, dispatcher](const QByteArray &message) {
            qDebug() << "Received:" << message;

            nativeMessaging->sendMessage(
                dispatcher->dispatch(message)
                );
        }
        );

    QObject::connect(
        nativeMessaging,
        &NativeMessaging::errorOccurred,
        nativeMessaging,
        [](const QString &error) {
            qWarning() << "Native Messaging error:" << error;
        }
        );

    QObject::connect(&app, &QApplication::aboutToQuit,
                      nativeMessaging, &NativeMessaging::stop);

    nativeMessaging->start();
}

} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto *localSocket = new QLocalSocket();

    if (tryConnectToPrimaryInstance(*localSocket)) {
        runAsBridge(app, localSocket);
    } else {
        delete localSocket;
        runAsPrimary(app);
    }

    return QApplication::exec();
}
