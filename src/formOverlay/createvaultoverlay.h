#ifndef CREATEVAULTOVERLAY_H
#define CREATEVAULTOVERLAY_H

#include <QWidget>

namespace Ui {
class CreateVaultOverlay;
}

class CreateVaultOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit CreateVaultOverlay(QWidget *parent = nullptr);
    ~CreateVaultOverlay();

    void clearForm();

signals:
    void cancelled();
    void vaultCreated(const QString &name, const QString &password);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::CreateVaultOverlay *ui;
};

#endif // CREATEVAULTOVERLAY_H