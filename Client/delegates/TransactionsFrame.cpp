#include "TransactionsFrame.h"
#include "ui_TransactionsFrame.h"

#include <QDateTime>
#include <QDebug>

TransactionsFrame::TransactionsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TransactionsFrame),
    bDb(DatabaseManager::instance())
{
    ui->setupUi(this);
    ui->tableView->setModel(&transactionModel);
}

TransactionsFrame::~TransactionsFrame()
{
    delete ui;
}

void TransactionsFrame::setHeaderData(const QSqlRecord &record, const bool &active)
{
    ui->bracer_number_label->setText(record.value("bracer_number").toString());
    ui->childs_label->setText(record.value("childs").toString());
    ui->enter_number_label->setText(record.value("enter_number").toString());
    ui->enter_time_label->setText(record.value("enter_time").toDateTime().toString("dd.MM.yyyy    hh:mm:ss"));

    if(active){
        ui->exit_number_label->setText(record.value("exit_number").toString());
        ui->exit_time_label->setText(record.value("exit_time").toDateTime().toString("dd.MM.yyyy    hh:mm:ss"));
    }
    ui->ballance_label->setText(record.value("cash").toString());
}

void TransactionsFrame::computeTransactions(const int &id, const bool &active)
{
    QString queryString =
            active?QString("SELECT active_transactions.title as Действие, products.title AS Продукт, active_transactions.quantity as Количество, active_transactions.time as Время, active_transactions.total_price as Сумма "
                           "FROM active_transactions "
                           "LEFT JOIN products ON active_transactions.product_id=products.id "
                           "WHERE active_transactions.active_bracers_id=%1;").arg(id)
                 :QString("SELECT transactions_history.title as Действие, products.title AS Продукт, transactions_history.quantity as Количество, transactions_history.time as Время, transactions_history.total_price as Сумма "
                          "FROM transactions_history "
                          "LEFT JOIN products ON transactions_history.product_id=products.id "
                          "WHERE transactions_history.bracers_history_id=%1;").arg(id);
    QSqlQuery query;
    if(!query.exec(queryString)){
        bDb.debugQuery(query);
        return;
    }
    transactionModel.setQuery(query);
    if(transactionModel.lastError().isValid())
        qDebug()<<transactionModel.lastError().text();

    double temp = 0;
    QSqlRecord rec;
    int rows = transactionModel.rowCount();

    for(int i=0; i<rows; i++){
        rec = transactionModel.record(i);
        temp+=rec.value("Сумма").toDouble();
    }
    ui->total_transaction_label->setText(QString::number(temp, 'f', 2));
}
