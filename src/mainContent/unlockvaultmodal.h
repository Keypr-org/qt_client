#ifndef UNLOCKVAULTMODAL_H
#define UNLOCKVAULTMODAL_H

#include <QWidget>

namespace Ui {
class unlockvaultmodal;
}

class unlockvaultmodal : public QWidget
{
    Q_OBJECT

public:
    explicit unlockvaultmodal(QWidget *parent = nullptr);
    ~unlockvaultmodal();

    void setVaultName(const QString &name);

signals:
    void vaultUnlocked(const QString &name); // emitted when the user successfully unlocks the vault

private slots:
    void onUnlockClicked();

private:
    Ui::unlockvaultmodal *ui;
    QString m_vaultName;
};

#endif // UNLOCKVAULTMODAL_H