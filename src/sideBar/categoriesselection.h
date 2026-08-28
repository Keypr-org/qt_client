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

signals:
    void lockVaultRequested();
    void setPersonaFrame();

private slots:
    void adjustListHeight();

    void on_listCategories_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::CategoriesSelection *ui;
    void updateArrowIcon(bool expanded);
};

#endif // CATEGORIESSELECTION_H
