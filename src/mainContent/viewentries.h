#ifndef VIEWENTRIES_H
#define VIEWENTRIES_H

#include <QWidget>

namespace Ui {
class ViewEntries;
}

class ViewEntries : public QWidget
{
    Q_OBJECT

public:
    explicit ViewEntries(QWidget *parent = nullptr);
    ~ViewEntries();

private:
    Ui::ViewEntries *ui;
};

#endif // VIEWENTRIES_H
