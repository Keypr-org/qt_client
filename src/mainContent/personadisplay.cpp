#include "personadisplay.h"
#include "ui_personadisplay.h"
#include "component/personaitem.h"
#include "vaultcontroller.h"
#include "../utils/qtypes/QPersona.h"

PersonaDisplay::PersonaDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::PersonaDisplay)
{
    ui->setupUi(this);

    ui->personasScrollArea->viewport()->setAutoFillBackground(true);
    ui->personasScrollArea->viewport()->setStyleSheet("background-color: #0F0F0A;");

    connect(ui->createPersonaButton, &QPushButton::clicked, this, [this]()
            { emit createPersona(); });
}

PersonaDisplay::~PersonaDisplay()
{
    delete ui;
}

void PersonaDisplay::loadPersonas()
{
    QLayoutItem *child;
    while ((child = ui->gridLayout->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    auto &controller = VaultController::getInstance();

    const auto personas = controller.getPersonas();
    for (int i = 0; i < personas.size(); ++i)
    {
        auto *item = new PersonaItem(ui->personasScrollContent);
        item->setPersona(personas.at(i));

        connect(item, &PersonaItem::modifyRequested, this, [this](const QPersona persona)
                { emit modifyPersonaRequested(persona); });

        connect(item, &PersonaItem::deleteRequested, this, [this](qint64 id)
                {
            auto &controller = VaultController::getInstance();
            if (controller.removePersona(id)) {
                loadPersonas();
            } });

        ui->gridLayout->addWidget(item, i / GRID_COLUMNS, i % GRID_COLUMNS);
    }
}

bool PersonaDisplay::addPersona(const QString &firstName, const QString &lastName,
                                const QDate &dateOfBirth, const QString &address,
                                const QString &phone)
{
    auto &controller = VaultController::getInstance();
    controller.addPersona(firstName, lastName, dateOfBirth, address, phone);
    loadPersonas();
    return true;
}

bool PersonaDisplay::updatePersona(qint64 id, const QString &firstName, const QString &lastName,
                                   const QDate &dateOfBirth, const QString &address,
                                   const QString &phone)
{
    auto &controller = VaultController::getInstance();
    if (!controller.updatePersona(id, firstName, lastName, dateOfBirth, address, phone))
    {
        return false;
    }

    loadPersonas();
    return true;
}
