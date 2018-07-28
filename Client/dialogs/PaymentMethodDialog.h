#ifndef PAYMENTMETHODDIALOG_H
#define PAYMENTMETHODDIALOG_H

#include <QDialog>

namespace Ui {
class PaymentMethodDialog;
}

class PaymentMethodDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaymentMethodDialog(const QString &title, QWidget *parent = 0);
    ~PaymentMethodDialog();

    bool isTerminal();
private:
    Ui::PaymentMethodDialog *ui;
};

#endif // PAYMENTMETHODDIALOG_H
