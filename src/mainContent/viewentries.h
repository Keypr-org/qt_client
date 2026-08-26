#ifndef VIEWENTRIES_H
#define VIEWENTRIES_H

#include <QWidget>
#include <QtWidgets/qlistwidget.h>

namespace Ui {
class ViewEntries;
}

class ViewEntries : public QWidget
{
    Q_OBJECT

public:
    explicit ViewEntries(QWidget *parent = nullptr);
    ~ViewEntries();

signals:
    void newEntry();

private slots:
    void on_entriesList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::ViewEntries *ui;
};

#endif // VIEWENTRIES_H
