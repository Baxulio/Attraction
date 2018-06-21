#ifndef SALESFORM_H
#define SALESFORM_H

#include <QWidget>
#include "DatabaseManager.h"
#include <QSqlTableModel>

#include "models/ProductsProxyModel.h"
#include "models/ProductTypesProxyModel.h"
#include "models/CartProxyModel.h"

namespace Ui {
class SalesForm;
}

class SalesForm : public QWidget
{
    Q_OBJECT

public:
    explicit SalesForm(QWidget *parent = 0);
    ~SalesForm();

private slots:
    void on_refresh();
    void on_back_but_clicked();

    void on_products_listView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_returnPressed();

    void on_clear_but_clicked();

    void on_search_but_clicked();

    void on_make_order_but_clicked();

private:
    Ui::SalesForm *ui;
    DatabaseManager &bDb;

    QSqlTableModel typesModel, productsModel;

    ProductTypesProxyModel productTypesProxyModel;
    ProductsProxyModel productsProxyModel;
    CartProxyModel cartProxyModel;
signals:
    void back();
};

#endif // SALESFORM_H
