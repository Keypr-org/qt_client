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
    explicit TextInput(QWidget *parent = nullptr);

    void setLabelText(const QString &text);
    void setInputPlaceholder(const QString &text);

    void setText(const QString &text);
    QString text() const;

    ~TextInput();

private:
    Ui::TextInput *ui;
};

#endif // TEXTINPUT_H
