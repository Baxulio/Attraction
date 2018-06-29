#ifndef DASHBOARDFORM_H
#define DASHBOARDFORM_H

#include <QWidget>
#include <QSqlTableModel>

#include "delegates/TransactionsFrame.h"
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

    void on_time_out_but_clicked(bool checked);

    void on_warning_but_clicked(bool checked);

private:
    Ui::DashboardForm *ui;
    TransactionsFrame *transactions;
    DatabaseManager &bDb;
    QSqlTableModel *activeModel;
    ProxyModel *proxyModel;

signals:
    void back();
};

#endif // DASHBOARDFORM_H
