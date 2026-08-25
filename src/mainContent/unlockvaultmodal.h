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

private:
    Ui::unlockvaultmodal *ui;
};

#endif // UNLOCKVAULTMODAL_H
