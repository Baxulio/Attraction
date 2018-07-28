#ifndef HISTORYFORM_H
#define HISTORYFORM_H

#include <QWidget>
#include <QSqlTableModel>

#include "delegates/TransactionsFrame.h"
#include "models/ProxyModel.h"

namespace Ui {
class HistoryForm;
}

class HistoryForm : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryForm(QWidget *parent = 0);
    ~HistoryForm();

private slots:
    void on_refresh();
    void on_back_but_clicked();
    void on_filter_but_clicked();
    void on_clean_but_clicked();

private:
    Ui::HistoryForm *ui;
    TransactionsFrame *transactions;
    DatabaseManager &bDb;
    QSqlTableModel *historyModel;
    ProxyModel *proxyModel;

private:
    void updateInfo();

signals:
    void back();
};

#endif // HISTORYFORM_H
