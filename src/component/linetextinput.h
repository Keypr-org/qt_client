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
    explicit LineTextInput(QWidget *parent = nullptr);

    void setLabelText(const QString &text);
    void setInputPlaceholder(const QString &text);

    ~LineTextInput();

private:
    Ui::LineTextInput *ui;
};

#endif // LINETEXTINPUT_H
