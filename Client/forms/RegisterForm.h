#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include "DatabaseManager.h"
#include <QSqlTableModel>

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
    QSqlTableModel *tariffModel;

signals:
    void back();

private slots:
    void on_refresh();
    void on_back_pushButton_clicked();
    void on_register_bracer_but_clicked();
    void on_drop_bracer_but_clicked();
    void on_tariff_comboBox_currentIndexChanged(int index);
};

#endif // REGISTERFORM_H