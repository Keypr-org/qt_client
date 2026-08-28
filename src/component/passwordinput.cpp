#include "passwordinput.h"
#include "ui_passwordinput.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QApplication>
#include <QClipboard>
#include <QUuid>

PasswordInput::PasswordInput(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordInput)
{
    ui->setupUi(this);

    ui->toggleButton->setIcon(QIcon(":/icons/icons/icon-eye.png"));
    ui->toggleButton->setToolTip(tr("Afficher/masquer le mot de passe"));

    connect(ui->toggleButton, &QToolButton::clicked, this, [this]() {
        if (ui->input->echoMode() == QLineEdit::Password)
            ui->input->setEchoMode(QLineEdit::Normal);
        else
            ui->input->setEchoMode(QLineEdit::Password);
    });
}

PasswordInput::~PasswordInput()
{
    delete ui;
}

void PasswordInput::setLabelText(const QString &text)
{
    ui->passwordLabel->setText(text);
}

QString PasswordInput::addAction(const QIcon &icon, const QString &tooltip, ActionCallback onClick)
{
    QToolButton *button = new QToolButton(this);
    button->setIcon(icon);
    button->setToolTip(tooltip);
    button->setCursor(Qt::PointingHandCursor);

    QString id = QUuid::createUuid().toString();
    button->setProperty("actionId", id);

    QLineEdit *target = ui->input;
    connect(button, &QToolButton::clicked, this, [target, onClick]() {
        if (onClick)
            onClick(target);
    });

    ui->horizontalLayout->addWidget(button);
    button->setStyleSheet("background: none; border:none;");

    return id;
}

void PasswordInput::removeAction(const QString &id)
{
    for (int i = 0; i < ui->horizontalLayout->count(); ++i) {
        QWidget *widget = ui->horizontalLayout->itemAt(i)->widget();
        if (widget && widget->property("actionId").toString() == id) {
            ui->horizontalLayout->removeWidget(widget);
            widget->deleteLater();
            break;
        }
    }
}

void PasswordInput::setEyeIconEnabled(bool enabled)
{
    ui->toggleButton->setVisible(enabled);
}

void PasswordInput::addCopyToClipboardAction()
{
    addAction(
        QIcon(":/icons/icons/icon-copy.png"),
        tr("Copier dans le presse-papier"),
        [](QLineEdit *target) {
            QApplication::clipboard()->setText(target->text());
        }
        );
}

void PasswordInput::setText(const QString &text)
{
    ui->input->setText(text);
}

QString PasswordInput::text() const
{
    return ui->input->text();
}

void PasswordInput::setLabelVisible(bool visible)
{
    ui->passwordLabel->setVisible(visible);
}