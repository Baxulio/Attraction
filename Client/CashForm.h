#ifndef CASHFORM_H
#define CASHFORM_H

#include <QWidget>

namespace Ui {
class CashForm;
}

class CashForm : public QWidget
{
    Q_OBJECT

public:
    explicit CashForm(QWidget *parent = 0);
    ~CashForm();

private:
    Ui::CashForm *ui;
};

#endif // CASHFORM_H
