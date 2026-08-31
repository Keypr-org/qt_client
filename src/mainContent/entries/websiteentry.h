#ifndef WEBSITEENTRY_H
#define WEBSITEENTRY_H

#include <QWidget>
#include <memory>

#include "model/websiteentrydata.h"

namespace Ui {
class WebsiteEntry;
}

class WebsiteEntry : public QWidget
{
    Q_OBJECT

public:
    explicit WebsiteEntry(QWidget *parent = nullptr);
    ~WebsiteEntry();

    void setEntry(const std::shared_ptr<WebsiteEntryData> &entry);

signals:
    void deleteRequested(QString id);
    void entryUpdated(QString id);

private:
    Ui::WebsiteEntry *ui;
    std::shared_ptr<WebsiteEntryData> m_entry;
};

#endif // WEBSITEENTRY_H
