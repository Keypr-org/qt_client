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
    explicit CategoriesSelection(QWidget *parent = nullptr);
    ~CategoriesSelection();

    void addCategory(const QString &name);
    void setVaultName(const QString &name);

signals:
    void lockVaultRequested();
    void setPersonaFrame();
    void categorySelected();
    void categoryReselected();
    void createCategoryRequested();

private slots:
    void adjustListHeight();

    void on_listCategories_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::CategoriesSelection *ui;
    QListWidgetItem *m_lastClickedCategory = nullptr;
    void updateArrowIcon(bool expanded);
    void setPersonaSelected(bool selected);
};

#endif // CATEGORIESSELECTION_H
