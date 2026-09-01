#include "personadisplay.h"
#include "ui_personadisplay.h"
#include "component/personaitem.h"

PersonaDisplay::PersonaDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonaDisplay)
{
    ui->setupUi(this);

    ui->personasScrollArea->viewport()->setAutoFillBackground(true);
    ui->personasScrollArea->viewport()->setStyleSheet("background-color: #0F0F0A;");

    connect(ui->createPersonaButton, &QPushButton::clicked, this, [this](){
        emit createPersona();
    });
}

PersonaDisplay::~PersonaDisplay()
{
    delete ui;
}

void PersonaDisplay::setVaultBridge(VaultBridge *bridge)
{
    m_vaultBridge = bridge;
}

void PersonaDisplay::loadPersonas()
{
    QLayoutItem *child;
    while ((child = ui->gridLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    if (!m_vaultBridge) {
        return;
    }

    const auto personas = m_vaultBridge->personas();
    for (int i = 0; i < personas.size(); ++i) {
        auto *item = new PersonaItem(ui->personasScrollContent);
        item->setPersona(personas.at(i));

        connect(item, &PersonaItem::modifyRequested, this, [this](VaultBridge::PersonaSummary persona){
            emit modifyPersonaRequested(persona);
        });

        connect(item, &PersonaItem::deleteRequested, this, [this](qint64 id){
            if (m_vaultBridge && m_vaultBridge->removePersona(id)) {
                loadPersonas();
            }
        });

        ui->gridLayout->addWidget(item, i / GRID_COLUMNS, i % GRID_COLUMNS);
    }
}

bool PersonaDisplay::addPersona(const QString &firstName, const QString &lastName,
                                 const QDate &dateOfBirth, const QString &address,
                                 const QString &phone)
{
    if (!m_vaultBridge) {
        return false;
    }

    if (m_vaultBridge->addPersona(firstName, lastName, dateOfBirth, address, phone) < 0) {
        return false;
    }

    loadPersonas();
    return true;
}

bool PersonaDisplay::updatePersona(qint64 id, const QString &firstName, const QString &lastName,
                                    const QDate &dateOfBirth, const QString &address,
                                    const QString &phone)
{
    if (!m_vaultBridge || !m_vaultBridge->updatePersona(id, firstName, lastName, dateOfBirth, address, phone)) {
        return false;
    }

    loadPersonas();
    return true;
}
