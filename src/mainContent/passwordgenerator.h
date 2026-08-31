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

    /**
     * @brief Generates a new password from the form's current options and displays it.
     * @return The generated password.
     */
    QString onGeneratePassword();

    /**
     * @brief Constructs the password generator widget.
     * @param parent Parent widget, or nullptr.
     */
    explicit PasswordGenerator(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and its UI.
     */
    ~PasswordGenerator();

private slots:
    /**
     * @brief Changes the desired password length whenever the slider is moved.
     * @param value New slider value (desired password length).
     */
    void on_passwordLengthSlider_valueChanged(int value);

signals:
    /**
     * @brief Emitted when the user cancels password generation.
     */
    void cancelRequested();

    /**
     * @brief Emitted when the user chooses to use the currently generated password.
     * @param password Password to hand back to the caller.
     */
    void usePasswordEvent(const QString password);

private:
    Ui::PasswordGenerator *ui;

    static constexpr const char* UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr const char* LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
    static constexpr const char* NUMBERS = "0123456789";
    static constexpr const char* SPECIAL_CHARS = "!@#$%^&*()_+-=[]{}|;:,.<>?";

    /**
     * @brief Builds a random password matching the requested length and character-set options.
     * @param length Total number of characters to generate.
     * @param useUppercase Whether uppercase letters may be used.
     * @param useLowercase Whether lowercase letters may be used.
     * @param useNumbers Whether digits may be used.
     * @param useSpecialChars Whether special characters may be used.
     * @return The generated password.
     */
    static QString generatePassword(
        int length,
        bool useUppercase,
        bool useLowercase,
        bool useNumbers,
        bool useSpecialChars
    );
};

#endif // PASSWORDGENERATOR_H
