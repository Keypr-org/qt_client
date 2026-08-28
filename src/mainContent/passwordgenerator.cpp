#include "passwordgenerator.h"
#include "component/toggleswitch.h"
#include "ui_passwordgenerator.h"

#include <QLineEdit>
#include <QRandomGenerator>

PasswordGenerator::PasswordGenerator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordGenerator)
{
    ui->setupUi(this);

    ui->passwordGenOutput->setLabelVisible(false);

    QString genId = ui->passwordGenOutput->addAction(
        QIcon(":/icons/icons/icon-regenerate.png"),
        "Générer un mot de passe",
        [this](QLineEdit *target) {
            onGeneratePassword();
        }
    );

    ui->passwordGenOutput->addCopyToClipboardAction();

    ui->toggle_1->setChecked(true);
    ui->toggle_2->setChecked(true);
    ui->toggle_3->setChecked(true);
    ui->toggle_4->setChecked(true);

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        emit cancelRequested();
    });

    connect(ui->usePasswordButton, &QPushButton::clicked, this, [this](){
        emit usePasswordEvent(ui->passwordGenOutput->text());
    });

    onGeneratePassword();
}

void PasswordGenerator::on_passwordLengthSlider_valueChanged(int value)
{
    ui->currentNumber->setText(QString::number(value));
}

QString PasswordGenerator::generatePassword(
    int length,
    bool useUppercase,
    bool useLowercase,
    bool useNumbers,
    bool useSpecialChars
    )
{
    // Build the available pool of available char
    QString availableChars;

    if (useUppercase)
        availableChars += UPPERCASE;
    if (useLowercase)
        availableChars += LOWERCASE;
    if (useNumbers)
        availableChars += NUMBERS;
    if (useSpecialChars)
        availableChars += SPECIAL_CHARS;

    if (availableChars.isEmpty())
        return QString();

    // Will be changed as soon as the correct method is implemented
    QString password;
    for (int i = 0; i < length; ++i) {
        int randomIndex = QRandomGenerator::global()->bounded(availableChars.length());
        password += availableChars[randomIndex];
    }

    return password;
}

QString PasswordGenerator::onGeneratePassword() {
    int length = ui->passwordLengthSlider->value();

    QString newPassword = PasswordGenerator::generatePassword(
        length,
        ui->toggle_1->isChecked(),
        ui->toggle_2->isChecked(),
        ui->toggle_3->isChecked(),
        ui->toggle_4->isChecked()
        );

    ui->passwordGenOutput->setText(newPassword);
    return newPassword;
}

PasswordGenerator::~PasswordGenerator()
{
    delete ui;
}
