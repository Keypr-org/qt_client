#include "nativemessagebridge.h"

#include <QLocalSocket>

#include "nativemessageframing.h"
#include "nativemessaging.h"

NativeMessageBridge::NativeMessageBridge(QLocalSocket *socket,
                                         NativeMessaging *nativeMessaging,
                                         QObject *parent)
    : QObject{parent}, m_socket{socket}, m_nativeMessaging{nativeMessaging},
      m_reassembler{new NativeMessageReassembler(this)} {
  m_socket->setParent(this);

  connect(m_socket, &QLocalSocket::readyRead, this,
          [this]() { m_reassembler->addData(m_socket->readAll()); });

  connect(m_reassembler, &NativeMessageReassembler::messageReady,
          m_nativeMessaging, &NativeMessaging::sendMessage);

  connect(m_nativeMessaging, &NativeMessaging::messageReceived, this,
          [this](const QByteArray &payload) {
            m_socket->write(frameNativeMessage(payload));
          });

  connect(m_socket, &QLocalSocket::disconnected, this,
          &NativeMessageBridge::primaryDisconnected);
}
