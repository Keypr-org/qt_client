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
    explicit CreateCategoryOverlay(QWidget *parent = nullptr);
    ~CreateCategoryOverlay();

    void clearForm();

signals:
    void cancelled();
    void categoryCreated(const QString &name);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::CreateCategoryOverlay *ui;
};

#endif // CREATECATEGORYOVERLAY_H
