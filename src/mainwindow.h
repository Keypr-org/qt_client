#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CreateVaultOverlay;
class VaultBridge;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    CreateVaultOverlay *m_createVaultOverlay;
    std::unique_ptr<VaultBridge> m_vaultBridge;

};
#endif // MAINWINDOW_H
