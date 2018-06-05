#ifndef HISTORYFORM_H
#define HISTORYFORM_H

#include <QWidget>
#include "delegates/TransactionsFrame.h"
#include <QSqlTableModel>
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

    void on_tariff_combo_currentIndexChanged(int index);

    void on_filter_but_clicked();

    void on_clean_but_clicked();

private:
    Ui::HistoryForm *ui;
    TransactionsFrame *transactions;
    DatabaseManager &bDb;
    QSqlTableModel *tariffModel, *historyModel;
    ProxyModel *proxyModel;

signals:
    void back();
};

#endif // HISTORYFORM_H
