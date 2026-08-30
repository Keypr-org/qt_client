#include "createcategoryoverlay.h"
#include "ui_createcategoryoverlay.h"

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
            return;
        }

        emit categoryCreated(name);
        clearForm();
        hide();
    });
}

void CreateCategoryOverlay::clearForm()
{
    ui->categoryName->setText("");
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
