#ifndef NOVAULTSELECTED_H
#define NOVAULTSELECTED_H

#include <QWidget>

namespace Ui {
class NoVaultSelected;
}

class NoVaultSelected : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the placeholder shown when no vault is selected.
     * @param parent Parent widget, or nullptr.
     */
    explicit NoVaultSelected(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and its UI.
     */
    ~NoVaultSelected();

private:
    Ui::NoVaultSelected *ui;
};

#endif // NOVAULTSELECTED_H
