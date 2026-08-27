#ifndef WEBSITEENTRY_H
#define WEBSITEENTRY_H

#include <QWidget>

namespace Ui {
class WebsiteEntry;
}

class WebsiteEntry : public QWidget
{
    Q_OBJECT

public:
    explicit WebsiteEntry(QWidget *parent = nullptr);
    ~WebsiteEntry();

private:
    Ui::WebsiteEntry *ui;
};

#endif // WEBSITEENTRY_H
