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

private:
    Ui::WebsiteEntry *ui;
    QString m_entryId;
};

#endif // WEBSITEENTRY_H
