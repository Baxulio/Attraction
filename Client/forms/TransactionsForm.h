#ifndef TRANSACTIONSFORM_H
#define TRANSACTIONSFORM_H

#include <QWidget>

#include "DatabaseManager.h"
#include <QSqlQueryModel>
#include "models/TransactionsProxyModel.h"

namespace Ui {
class TransactionsForm;
}

class TransactionsForm : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsForm(QWidget *parent = 0);
    ~TransactionsForm();

private slots:
    void on_refresh();
    void on_back_but_clicked();
    void on_filter_but_triggered(QAction *arg1);
    void on_clean_but_clicked();

    void on_today_but_clicked();

    void on_filter_but_clicked();

private:
    Ui::TransactionsForm *ui;
    DatabaseManager &bDb;
    QSqlQueryModel *model;
    TransactionsProxyModel *proxyModel;

private:
    void updateInfo();
    void clearFields();
    void invalidateProxyModel();
signals:
    void back();
};

#endif // TRANSACTIONSFORM_H
