#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include "DatabaseManager.h"

#include "dialogs/SettingsDialog.h"

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(SettingsDialog &set, QWidget *parent = 0);
    ~RegisterForm();

private:
    Ui::RegisterForm *ui;
    DatabaseManager &bDb;
    SettingsDialog &bSetings;

private:
    void updatePriceLabel();
signals:
    void back();

private slots:
    void on_refresh();

    void on_back_pushButton_clicked();
    void on_register_bracer_but_clicked();
    void on_drop_bracer_but_clicked();
    void on_bracer_number_spinBox_valueChanged(int arg1);
    void on_childs_spinBox_valueChanged(int arg1);
    void on_tariff_comboBox_currentIndexChanged(int index);
    void on_resetFields_but_clicked();
    void on_cash_doubleSpinBox_valueChanged(double arg1);
};

#endif // REGISTERFORM_H
