#include "appconfig.h"
#include "mainwindow.h"
#include "vaultcontroller.h"
#include "vaultstoragesetupdialog.h"

#include <QApplication>
#include <QEvent>
#include <QFont>
#include <QGuiApplication>
#include <QScreen>
#include <QWidget>

namespace {
void applyPlatformFont()
{
#if defined(Q_OS_WIN)
    QFont font;
    font.setFamilies({"Segoe UI", "Segoe UI Variable Text", "Tahoma", "sans-serif"});
    QApplication::setFont(font);
#elif defined(Q_OS_LINUX)
    QFont font;
    font.setFamilies({"Noto Sans", "Cantarell", "Ubuntu", "DejaVu Sans", "sans-serif"});
    QApplication::setFont(font);
#endif
}

// Qt resolves QFont point sizes against the screen's logical DPI (72 on macOS,
// 96 on Windows/Linux), so identical .ui-file point sizes render ~33% bigger
// off macOS. Compensate by rescaling every widget's font the first time it is
// polished, instead of hand-editing point sizes in ~20 .ui files.
qreal dpiFontScaleFactor()
{
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        if (screen->logicalDotsPerInch() > 0) {
            return 72.0 / screen->logicalDotsPerInch();
        }
    }
#endif
    return 1.0;
}

class FontDpiCompensator : public QObject
{
public:
    explicit FontDpiCompensator(qreal factor, QObject *parent = nullptr)
        : QObject(parent), m_factor(factor)
    {
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
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
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Keypr");
    QApplication::setApplicationDisplayName("Keypr");

    applyPlatformFont();

    const qreal fontScale = dpiFontScaleFactor();
    if (!qFuzzyCompare(fontScale, 1.0)) {
        a.installEventFilter(new FontDpiCompensator(fontScale, &a));
    }

    AppConfig config = AppConfig::load();
    if (!config.hasValidVaultStoragePath()) {
        VaultStorageSetupDialog setupDialog;
        setupDialog.exec();
    }

    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.showMaximized();

    QObject::connect(&a, &QApplication::aboutToQuit, [&]()
    {
        auto &vaultController = VaultController::getInstance();
        if (vaultController.isVaultUnlocked()) {
            vaultController.lockVault();
        }
    });

    return QApplication::exec();
}
