#ifndef VIEWENTRIES_H
#define VIEWENTRIES_H

#include <QWidget>
#include <QtWidgets/qlistwidget.h>
#include <QStackedWidget>
#include <QString>
#include <memory>

#include "model/entry.h"
#include "model/entryrepository.h"

class WebsiteEntry;
class WifiEntry;
class CreditCardEntry;

namespace Ui {
class ViewEntries;
}

class ViewEntries : public QWidget
{
    Q_OBJECT

public:
    explicit ViewEntries(QWidget *parent = nullptr);
    ~ViewEntries();

    EntryRepository *repository() const;
    void refresh();

    void clearSelection();

signals:
    void newEntry();

private slots:
    void on_entriesList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::ViewEntries *ui;
    EntryRepository *m_repository;

    QStackedWidget *m_detailStack;
    QWidget *m_emptyDetailPage;
    WebsiteEntry *m_websiteEntryView;
    WifiEntry *m_wifiEntryView;
    CreditCardEntry *m_creditCardEntryView;

    void populateList();
    void showEntryDetails(const std::shared_ptr<Entry> &entry);
    void handleDeleteRequested(const QString &id);
    void handleEntryUpdated(const QString &id);
};

#endif // VIEWENTRIES_H
