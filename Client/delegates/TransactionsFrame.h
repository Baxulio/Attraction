#ifndef TRANSACTIONSFRAME_H
#define TRANSACTIONSFRAME_H

#include <QFrame>

#include "DatabaseManager.h"
#include <QSqlQueryModel>
#include <QSqlRecord>

namespace Ui {
class TransactionsFrame;
}

class TransactionsFrame : public QFrame
{
    Q_OBJECT
public:
    QSqlQueryModel transactionModel;
public:
    explicit TransactionsFrame(QWidget *parent = 0);
    ~TransactionsFrame();

    void setHeaderData(const QSqlRecord &record, const bool &active);
    void computeTransactions(const int &id, const bool &active);

public slots:
    void on_print_but_clicked();

private:
    Ui::TransactionsFrame *ui;
    DatabaseManager &bDb;
};

#endif // TRANSACTIONSFRAME_H
