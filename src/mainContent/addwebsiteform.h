#ifndef ADDWEBSITEFORM_H
#define ADDWEBSITEFORM_H

#include <QWidget>
#include <memory>

#include "model/websiteentrydata.h"

namespace Ui {
class AddWebsiteForm;
}

class AddWebsiteForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddWebsiteForm(QWidget *parent = nullptr);
    ~AddWebsiteForm();

    void setPassword(const QString &password);
    void clearForm();

signals:
    void generatePassword();
    void createWebsiteEntry(std::shared_ptr<WebsiteEntryData> entry);
    void cancelNewWebEntry();

private:
    Ui::AddWebsiteForm *ui;
};

#endif // ADDWEBSITEFORM_H
