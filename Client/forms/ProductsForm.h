#ifndef PRODUCTSFORM_H
#define PRODUCTSFORM_H

#include <QWidget>
#include "DatabaseManager.h"

#include <QSortFilterProxyModel>
#include "models/CustomSqlTableModel.h"

namespace Ui {
class ProductsForm;
}

class ProductsForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductsForm(QWidget *parent = 0);
    ~ProductsForm();

private slots:
    void on_refresh();
    void on_back_but_clicked();

    void on_types_revert_but_clicked();

    void on_types_submit_but_clicked();

    void on_add_product_type_but_clicked();

    void on_delete_product_type_but_clicked();

    void on_change_product_type_icon_but_clicked();

    void on_add_product_but_clicked();

    void on_delete_product_but_clicked();

    void on_change_product_icon_but_clicked();


    void on_product_suubmit_but_clicked();

    void on_product_revert_but_clicked();

private:
    Ui::ProductsForm *ui;
    DatabaseManager &bDb;
    CustomSqlTableModel *product_types_model;
    CustomSqlTableModel *products_model;

    QSortFilterProxyModel *productsProxyModel;


signals:
    void back();
};

#endif // PRODUCTSFORM_H
