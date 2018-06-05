#ifndef DASHBOARDFORM_H
#define DASHBOARDFORM_H

#include <QWidget>
#include "delegates/TransactionsFrame.h"
#include <QSqlTableModel>
#include "models/ProxyModel.h"

namespace Ui {
class DashboardForm;
}

class DashboardForm : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardForm(QWidget *parent = 0);
    ~DashboardForm();

private slots:
    void on_refresh();

    void on_back_but_clicked();

    void on_clean_but_clicked();

    void on_filter_but_clicked();

    void on_tariff_combo_currentIndexChanged(int index);

private:
    Ui::DashboardForm *ui;
    TransactionsFrame *transactions;
    DatabaseManager &bDb;
    QSqlTableModel *tariffModel, *activeModel;
    ProxyModel *proxyModel;

signals:
    void back();
};

#endif // DASHBOARDFORM_H
