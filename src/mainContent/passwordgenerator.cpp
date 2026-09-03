#include "passwordgenerator.h"
#include "component/passwordinput.h"
#include "component/toggleswitch.h"
#include "ui_passwordgenerator.h"
#include "utils/passwordstrengthutils.h"

#include <QLineEdit>
#include <QRandomGenerator>
#include <QStringList>

#include <utility>

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

    connect(ui->passwordGenOutput, &PasswordInput::textChanged, this, &PasswordGenerator::updateStrengthDisplay);

    onGeneratePassword();
}

void PasswordGenerator::on_passwordLengthSlider_valueChanged(int value)
{
    ui->currentNumber->setText(QString::number(value));
}

void PasswordGenerator::updateStrengthDisplay(const QString &password)
{
    const PasswordStrengthUtils::Strength strength = PasswordStrengthUtils::evaluate(password);
    const QString color = PasswordStrengthUtils::color(strength);
    const int filledBars = PasswordStrengthUtils::filledBarCount(strength);

    ui->label_3->setText(PasswordStrengthUtils::label(strength));
    ui->label_3->setStyleSheet("color: " + color + ";");

    QWidget *bars[] = { ui->bar_1, ui->bar_2, ui->bar_3, ui->bar_4 };
    for (int i = 0; i < 4; ++i) {
        const QString barColor = i < filledBars ? color : "#1F2937";
        bars[i]->setStyleSheet("background-color: " + barColor + "; border-radius: 5px;");
    }
}

QString PasswordGenerator::generatePassword(
    int length,
    bool useUppercase,
    bool useLowercase,
    bool useNumbers,
    bool useSpecialChars
    )
{
    QStringList selectedPools;

    if (useUppercase)
        selectedPools << UPPERCASE;
    if (useLowercase)
        selectedPools << LOWERCASE;
    if (useNumbers)
        selectedPools << NUMBERS;
    if (useSpecialChars)
        selectedPools << SPECIAL_CHARS;

    if (selectedPools.isEmpty())
        return QString();

    QString availableChars = selectedPools.join(QString());

    // Guarantee at least one character from each selected class
    QString password;
    for (const QString &pool : std::as_const(selectedPools)) {
        if (password.length() >= length)
            break;
        password += pool.at(QRandomGenerator::global()->bounded(pool.length()));
    }

    while (password.length() < length) {
        password += availableChars.at(QRandomGenerator::global()->bounded(availableChars.length()));
    }

    // Shuffle so the guaranteed characters aren't always at the front
    for (int i = password.length() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        const QChar tmp = password[i];
        password[i] = password[j];
        password[j] = tmp;
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
