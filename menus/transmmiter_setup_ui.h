#ifndef TRANSMMITER_SETUP_UI_H
#define TRANSMMITER_SETUP_UI_H

#include <QWidget>

namespace Ui {
class transmmiter_setup_ui;
}

class transmmiter_setup_ui : public QWidget
{
    Q_OBJECT

public:
    explicit transmmiter_setup_ui(QWidget *parent = nullptr);
    ~transmmiter_setup_ui();

private:
    Ui::transmmiter_setup_ui *ui;
};

#endif // TRANSMMITER_SETUP_UI_H
