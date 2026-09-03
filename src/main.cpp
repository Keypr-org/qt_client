#include "appconfig.h"
#include "mainwindow.h"
#include "nativeMessaging/nativemessagebridge.h"
#include "nativeMessaging/nativemessagedispatcher.h"
#include "nativeMessaging/nativemessageserver.h"
#include "nativeMessaging/nativemessaging.h"
#include "vaultcontroller.h"
#include "vaultstoragesetupdialog.h"

#include <QApplication>
#include <QEvent>
#include <QFont>
#include <QGuiApplication>
#include <QLocalSocket>
#include <QScreen>
#include <QWidget>

namespace {
void applyPlatformFont() {
#if defined(Q_OS_WIN)
  QFont font;
  font.setFamilies(
      {"Segoe UI", "Segoe UI Variable Text", "Tahoma", "sans-serif"});
  QApplication::setFont(font);
#elif defined(Q_OS_LINUX)
  QFont font;
  font.setFamilies(
      {"Noto Sans", "Cantarell", "Ubuntu", "DejaVu Sans", "sans-serif"});
  QApplication::setFont(font);
#endif
}

// Qt resolves QFont point sizes against the screen's logical DPI (72 on macOS,
// 96 on Windows/Linux), so identical .ui-file point sizes render ~33% bigger
// off macOS. Compensate by rescaling every widget's font the first time it is
// polished, instead of hand-editing point sizes in ~20 .ui files.
qreal dpiFontScaleFactor() {
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
  if (QScreen *screen = QGuiApplication::primaryScreen()) {
    if (screen->logicalDotsPerInch() > 0) {
      return 72.0 / screen->logicalDotsPerInch();
    }
  }
#endif
  return 1.0;
}

class FontDpiCompensator : public QObject {
public:
  explicit FontDpiCompensator(qreal factor, QObject *parent = nullptr)
      : QObject(parent), m_factor(factor) {}

protected:
  bool eventFilter(QObject *watched, QEvent *event) override {
    if (event->type() == QEvent::Polish) {
      if (auto *widget = qobject_cast<QWidget *>(watched)) {
        if (!widget->property("dpiFontScaled").toBool()) {
          QFont f = widget->font();
          if (f.pointSizeF() > 0) {
            f.setPointSizeF(f.pointSizeF() * m_factor);
            widget->setFont(f);
          }
          widget->setProperty("dpiFontScaled", true);
        }
      }
    }
    return QObject::eventFilter(watched, event);
  }

private:
  qreal m_factor;
};

constexpr int kInstanceProbeTimeoutMs = 200;

// True if another qt_client process is already answering as the primary
// instance. On success, socket stays connected and can be reused by the
// caller instead of reconnecting.
bool tryConnectToPrimaryInstance(QLocalSocket &socket) {
  socket.connectToServer(NativeMessageServer::serverName());
  return socket.waitForConnected(kInstanceProbeTimeoutMs);
}

// Chrome spawns a brand-new qt_client process on every
// chrome.runtime.connectNative() call. When a primary instance already
// exists, this process only relays Native Messaging bytes between Chrome
// (via stdio) and the primary (via localSocket); it never shows a window.
void runAsBridge(QApplication &app, QLocalSocket *localSocket) {
  auto *nativeMessaging = new NativeMessaging(&app);
  auto *bridge = new NativeMessageBridge(localSocket, nativeMessaging, &app);

  // If the main qt_client exits the other clients
  QObject::connect(bridge, &NativeMessageBridge::primaryDisconnected, &app,
                   &QApplication::quit);
  QObject::connect(nativeMessaging, &NativeMessaging::finished, &app,
                   &QApplication::quit);
  QObject::connect(&app, &QApplication::aboutToQuit, nativeMessaging,
                   &NativeMessaging::stop);

  nativeMessaging->start();
}

void runAsPrimary(QApplication &app) {
  AppConfig config = AppConfig::load();
  if (!config.hasValidVaultStoragePath()) {
    VaultStorageSetupDialog setupDialog;
    setupDialog.exec();
  }

  // Create controller instance so we ensure it has a valid state with the
  // storage path
  VaultController::getInstance(config.vaultStoragePath);

  // Create native messaging related objects
  auto *nativeMessaging = new NativeMessaging(&app);
  auto *dispatcher = new NativeMessageDispatcher(&app);
  auto *server = new NativeMessageServer(dispatcher, &app);

  auto *w = new MainWindow();
  w->setWindowTitle("Keypr | Your local password manager");
  w->showMaximized();

  // Fix font issues on some platforms
  applyPlatformFont();

  const qreal fontScale = dpiFontScaleFactor();
  if (!qFuzzyCompare(fontScale, 1.0)) {
    app.installEventFilter(new FontDpiCompensator(fontScale, &app));
  }

  // Start the native message server to receive message that are beeing relayed
  // to the main instance by bridge instances
  if (!server->start()) {
    qWarning()
        << "Could not start the Native Messaging instance server; "
           "other qt_client processes won't be able to reuse this instance.";
  }

  // Connect Native Messaging signals to the corresponding slots
  QObject::connect(
      nativeMessaging, &NativeMessaging::messageReceived, nativeMessaging,
      [nativeMessaging, dispatcher](const QByteArray &message) {
        qDebug() << "Received:" << message;

        nativeMessaging->sendMessage(dispatcher->dispatch(message));
      });

  QObject::connect(nativeMessaging, &NativeMessaging::errorOccurred,
                   nativeMessaging, [](const QString &error) {
                     qWarning() << "Native Messaging error:" << error;
                   });

  QObject::connect(&app, &QApplication::aboutToQuit, nativeMessaging,
                   &NativeMessaging::stop);

  // Ensure the vault is saved before the application exits
  QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
    auto &vaultController = VaultController::getInstance();
    if (vaultController.isVaultUnlocked()) {
      vaultController.lockVault();
    }
  });

  nativeMessaging->start();
}

} // namespace

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  auto *localSocket = new QLocalSocket();

  // If another qt_client process is already running, this process will act as a
  // bridge
  if (tryConnectToPrimaryInstance(*localSocket)) {
    runAsBridge(app, localSocket);
  } else {
    delete localSocket;
    runAsPrimary(app);
  }

  return QApplication::exec();
}
