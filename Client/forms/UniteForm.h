#ifndef UNITEFORM_H
#define UNITEFORM_H

#include <QWidget>
#include "DatabaseManager.h"

namespace Ui {
class UniteForm;
}

class UniteForm : public QWidget
{
    Q_OBJECT

public:
    explicit UniteForm(QWidget *parent = 0);
    ~UniteForm();

private:
    Ui::UniteForm *ui;
    DatabaseManager &bDb;

private slots:
    void on_back_but_clicked();
    void on_add_product_type_but_clicked();
    void on_delete_product_type_but_clicked();
    void on_unite_bracers_pushButton_clicked();

signals:
    void back();
};

#endif // UNITEFORM_H
