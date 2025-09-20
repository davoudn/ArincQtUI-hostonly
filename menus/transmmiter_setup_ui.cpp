#include "transmmiter_setup_ui.h"
#include "ui_transmmiter_setup_ui.h"

transmmiter_setup_ui::transmmiter_setup_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::transmmiter_setup_ui)
{
    ui->setupUi(this);
}

transmmiter_setup_ui::~transmmiter_setup_ui()
{
    delete ui;
}
