#include "nativemessageserver.h"

#include <QLocalServer>
#include <QLocalSocket>

#include "nativemessagedispatcher.h"
#include "nativemessageframing.h"

NativeMessageServer::NativeMessageServer(NativeMessageDispatcher *dispatcher, QObject *parent)
    : QObject{parent}
    , m_server{new QLocalServer(this)}
    , m_dispatcher{dispatcher}
{
    connect(m_server, &QLocalServer::newConnection,
            this, &NativeMessageServer::handleNewConnection);
}

QString NativeMessageServer::serverName()
{
    return QStringLiteral("com.keypr.native.instance");
}

bool NativeMessageServer::start()
{
    // Only called after the caller has already confirmed no other instance
    // answers on this name, so any leftover socket file is stale.
    QLocalServer::removeServer(serverName());

    return m_server->listen(serverName());
}

void NativeMessageServer::handleNewConnection()
{
    while (QLocalSocket *socket = m_server->nextPendingConnection()) {
        auto *reassembler = new NativeMessageReassembler(socket);

        connect(socket, &QLocalSocket::readyRead, this, [socket, reassembler]() {
            reassembler->addData(socket->readAll());
        });

        connect(reassembler, &NativeMessageReassembler::messageReady, this,
                [this, socket](const QByteArray &payload) {
                    socket->write(frameNativeMessage(m_dispatcher->dispatch(payload)));
                });

        connect(socket, &QLocalSocket::disconnected, socket, &QObject::deleteLater);
    }
}
