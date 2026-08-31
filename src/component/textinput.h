#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <QWidget>

namespace Ui {
class TextInput;
}

class TextInput : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a multi-line labeled text input.
     * @param parent Parent widget.
     */
    explicit TextInput(QWidget *parent = nullptr);

    /**
     * @brief Sets the field's label text.
     * @param text The label to display.
     */
    void setLabelText(const QString &text);
    /**
     * @brief Sets the placeholder text shown when the field is empty.
     * @param text The placeholder text.
     */
    void setInputPlaceholder(const QString &text);

    /**
     * @brief Sets the field's current text content.
     * @param text The text to set.
     */
    void setText(const QString &text);
    /**
     * @brief Returns the field's current text content.
     * @return The current text.
     */
    QString text() const;

    /**
     * @brief Destroys the widget and its UI.
     */
    ~TextInput();

private:
    Ui::TextInput *ui;
};

#endif // TEXTINPUT_H
