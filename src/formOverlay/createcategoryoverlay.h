#ifndef CREATECATEGORYOVERLAY_H
#define CREATECATEGORYOVERLAY_H

#include <QWidget>

namespace Ui {
class CreateCategoryOverlay;
}

class CreateCategoryOverlay : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the category creation overlay.
     * @param parent Parent widget.
     */
    explicit CreateCategoryOverlay(QWidget *parent = nullptr);

    /**
     * @brief Destroys the overlay and releases its UI resources.
     */
    ~CreateCategoryOverlay();

    /**
     * @brief Resets the form fields to their empty/default state.
     */
    void clearForm();

signals:
    /**
     * @brief Emitted when the user cancels category creation.
     */
    void cancelled();

    /**
     * @brief Emitted when the user submits a valid new category.
     * @param name Name of the category to create.
     */
    void categoryCreated(const QString &name);

protected:
    /**
     * @brief Keeps the overlay's geometry in sync with its parent when resized.
     * @param event Resize event data.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::CreateCategoryOverlay *ui;
};

#endif // CREATECATEGORYOVERLAY_H
