#include "personaitem.h"
#include "ui_personaitem.h"

#include <QPainter>
#include <QStyleOption>

PersonaItem::PersonaItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonaItem)
{
    ui->setupUi(this);
}

PersonaItem::~PersonaItem()
{
    delete ui;
}

void PersonaItem::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
