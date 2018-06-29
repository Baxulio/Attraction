#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include "DatabaseManager.h"

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = 0);
    ~RegisterForm();

private:
    Ui::RegisterForm *ui;
    DatabaseManager &bDb;

signals:
    void back();

private slots:
    void on_back_pushButton_clicked();
    void on_register_bracer_but_clicked();
    void on_drop_bracer_but_clicked();
    void on_bracer_number_spinBox_valueChanged(int arg1);
    void on_childs_spinBox_valueChanged(int arg1);
};

#endif // REGISTERFORM_H
