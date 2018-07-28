#include "PaymentMethodDialog.h"
#include "ui_PaymentMethodDialog.h"

PaymentMethodDialog::PaymentMethodDialog(const QString &title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaymentMethodDialog)
{
    ui->setupUi(this);
    ui->label->setText(title);
}

PaymentMethodDialog::~PaymentMethodDialog()
{
    delete ui;
}

bool PaymentMethodDialog::isTerminal()
{
    return ui->terminal_radio->isChecked();
}
