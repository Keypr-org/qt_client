#ifndef LINETEXTINPUT_H
#define LINETEXTINPUT_H

#include <QWidget>

namespace Ui {
class LineTextInput;
}

class LineTextInput : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a single-line labeled text input.
     * @param parent Parent widget.
     */
    explicit LineTextInput(QWidget *parent = nullptr);

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
    ~LineTextInput();

signals:
    /**
     * @brief Emitted whenever the field's text value changes.
     * @param text The new text.
     */
    void textChanged(QString text);

private slots:
    /**
     * @brief Handles user edits to the underlying input field and re-emits the change.
     * @param arg1 The current text of the input field.
     */
    void on_input_textEdited(const QString &arg1);

private:
    Ui::LineTextInput *ui;
};

#endif // LINETEXTINPUT_H
