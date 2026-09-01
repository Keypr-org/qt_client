#ifndef CATEGORIESSELECTION_H
#define CATEGORIESSELECTION_H

#include <QWidget>
#include <QtWidgets/qlistwidget.h>

namespace Ui {
class CategoriesSelection;
}

class CategoriesSelection : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief A category as displayed in the list: its real identifier plus display name.
     */
    struct CategoryItem {
        qint64 id;
        QString name;
    };

    /**
     * @brief Constructs the sidebar categories selection panel.
     * @param parent Parent widget.
     */
    explicit CategoriesSelection(QWidget *parent = nullptr);

    /**
     * @brief Destroys the panel and releases its UI resources.
     */
    ~CategoriesSelection();

    /**
     * @brief Appends a new category entry to the list.
     * @param id Identifier of the category to add.
     * @param name Name of the category to add.
     */
    void addCategory(qint64 id, const QString &name);

    /**
     * @brief Replaces the displayed categories with the given list, selecting the first one
     * (if any) to match the initial-selection behavior of the previous placeholder list.
     * @param categories Categories to display, in order.
     */
    void setCategories(const QList<CategoryItem> &categories);

    /**
     * @brief Updates the displayed name of the currently open vault.
     * @param name Vault name to display.
     */
    void setVaultName(const QString &name);

signals:
    /**
     * @brief Emitted when the user requests to lock the current vault.
     */
    void lockVaultRequested();

    /**
     * @brief Emitted when the user selects the special "Personas" frame.
     */
    void setPersonaFrame();

    /**
     * @brief Emitted when the user selects a different category.
     * @param categoryId Identifier of the newly selected category.
     */
    void categorySelected(qint64 categoryId);

    /**
     * @brief Emitted when the user re-selects the already-selected category.
     */
    void categoryReselected();

    /**
     * @brief Emitted when the user requests creation of a new category.
     */
    void createCategoryRequested();

private slots:
    /**
     * @brief Recomputes and applies the categories list widget's height to fit its content.
     */
    void adjustListHeight();

    /**
     * @brief Handles a change of the selected category item, updating selection state and emitting the matching signal.
     * @param current Newly selected item, or nullptr if selection was cleared.
     * @param previous Previously selected item, or nullptr if there was none.
     */
    void on_listCategories_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::CategoriesSelection *ui;
    QListWidgetItem *m_lastClickedCategory = nullptr;

    /**
     * @brief Clears the icon and selection state of every category row (e.g. when switching to
     * the Personas frame), forcing an immediate repaint.
     */
    void deselectAllCategories();

    /**
     * @brief Swaps the expand/collapse arrow icon to reflect the panel's expanded state.
     * @param expanded Whether the panel is currently expanded.
     */
    void updateArrowIcon(bool expanded);

    /**
     * @brief Applies or clears the visual "selected" styling on the Personas entry.
     * @param selected Whether the Personas entry should appear selected.
     */
    void setPersonaSelected(bool selected);
};

#endif // CATEGORIESSELECTION_H
