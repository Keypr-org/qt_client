#include "passwordstrengthutils.h"

namespace
{

const QString COLOR_WEAK       = "#EF4444";
const QString COLOR_POOR       = "#F59E0B";
const QString COLOR_STRONG     = "#38BDF8";
const QString COLOR_VERYSTRONG = "#10B981";

int countCharacterClasses(const QString &password)
{
    bool hasLower = false;
    bool hasUpper = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (const QChar &c : password) {
        if (c.isLower())
            hasLower = true;
        else if (c.isUpper())
            hasUpper = true;
        else if (c.isDigit())
            hasDigit = true;
        else
            hasSpecial = true;
    }

    return (hasLower ? 1 : 0) + (hasUpper ? 1 : 0) + (hasDigit ? 1 : 0) + (hasSpecial ? 1 : 0);
}

}

PasswordStrengthUtils::Strength PasswordStrengthUtils::evaluate(const QString &password)
{
    const int length = password.length();
    const int classes = countCharacterClasses(password);

    if (length < 8 || (classes <= 1 && length < 16))
        return Strength::Weak;

    if (classes == 4 && length >= 16)
        return Strength::VeryStrong;

    if (classes >= 3 && length >= 12)
        return Strength::Strong;

    return Strength::Poor;
}

QString PasswordStrengthUtils::label(Strength strength)
{
    switch (strength) {
    case Strength::Weak:
        return "Weak";
    case Strength::Poor:
        return "Poor";
    case Strength::Strong:
        return "Strong";
    case Strength::VeryStrong:
        return "Very Strong";
    }

    return QString();
}

QString PasswordStrengthUtils::color(Strength strength)
{
    switch (strength) {
    case Strength::Weak:
        return COLOR_WEAK;
    case Strength::Poor:
        return COLOR_POOR;
    case Strength::Strong:
        return COLOR_STRONG;
    case Strength::VeryStrong:
        return COLOR_VERYSTRONG;
    }

    return QString();
}

int PasswordStrengthUtils::filledBarCount(Strength strength)
{
    switch (strength) {
    case Strength::Weak:
        return 1;
    case Strength::Poor:
        return 2;
    case Strength::Strong:
        return 3;
    case Strength::VeryStrong:
        return 4;
    }

    return 0;
}
