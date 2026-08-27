#include "websiteentry.h"
#include "ui_websiteentry.h"

WebsiteEntry::WebsiteEntry(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WebsiteEntry)
{
    ui->setupUi(this);
}

WebsiteEntry::~WebsiteEntry()
{
    delete ui;
}
