#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <QWidget>

namespace Ui {
class PasswordGenerator;
}

class PasswordGenerator : public QWidget
{
    Q_OBJECT

public:

    QString onGeneratePassword();

    explicit PasswordGenerator(QWidget *parent = nullptr);

    ~PasswordGenerator();

private slots:
    void on_passwordLengthSlider_valueChanged(int value);

signals:
    void cancelRequested();
    void usePasswordEvent(const QString password);

private:
    Ui::PasswordGenerator *ui;

    static constexpr const char* UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr const char* LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
    static constexpr const char* NUMBERS = "0123456789";
    static constexpr const char* SPECIAL_CHARS = "!@#$%^&*()_+-=[]{}|;:,.<>?";

    static QString generatePassword(
        int length,
        bool useUppercase,
        bool useLowercase,
        bool useNumbers,
        bool useSpecialChars
    );
};

#endif // PASSWORDGENERATOR_H
