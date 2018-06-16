#ifndef SALESFORM_H
#define SALESFORM_H

#include <QWidget>
#include "DatabaseManager.h"
#include "models/SalesModel.h"
#include <QSortFilterProxyModel>

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

private:
    Ui::SalesForm *ui;
    DatabaseManager &bDb;

    SalesModel typesModel, productsModel;
    QSortFilterProxyModel productsProxyModel;

signals:
    void back();
};

#endif // SALESFORM_H
