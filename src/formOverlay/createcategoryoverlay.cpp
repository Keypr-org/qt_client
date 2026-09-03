#include "createcategoryoverlay.h"
#include "ui_createcategoryoverlay.h"

#include "component/notificationtooltip.h"

CreateCategoryOverlay::CreateCategoryOverlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateCategoryOverlay)
{
    ui->setupUi(this);
    ui->categoryName->setLabelText("CATEGORY NAME");

    // By default, hide the overlay
    hide();

    connect(ui->cancelCategory, &QPushButton::clicked, this, [this]() {
        clearForm();
        emit cancelled();
        hide();
    });

    connect(ui->createCategoryButton, &QPushButton::clicked, this, [this]() {
        const QString name = ui->categoryName->text();

        if (name.isEmpty()) {
            ui->notificationTooltip->showError("Please enter a category name.");
            return;
        }

        emit categoryCreated(name);
    });
}

void CreateCategoryOverlay::clearForm()
{
    ui->categoryName->setText("");
    ui->notificationTooltip->hideMessage();
}

void CreateCategoryOverlay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (parentWidget()) {
        setGeometry(parentWidget()->rect());
    }
}

CreateCategoryOverlay::~CreateCategoryOverlay()
{
    delete ui;
}
