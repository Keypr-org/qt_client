#ifndef ADDWEBSITEFORM_H
#define ADDWEBSITEFORM_H

#include <QWidget>

namespace Ui {
class AddWebsiteForm;
}

class AddWebsiteForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddWebsiteForm(QWidget *parent = nullptr);
    ~AddWebsiteForm();

signals:
    void generatePassword();
    void createWebsiteEntry();
    void cancelNewWebEntry();

private:
    Ui::AddWebsiteForm *ui;
};

#endif // ADDWEBSITEFORM_H
