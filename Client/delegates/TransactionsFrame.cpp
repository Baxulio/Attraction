#include "TransactionsFrame.h"
#include "ui_TransactionsFrame.h"

#include <QDateTime>
#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextStream>

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
        ui->exit_number_label->setText(record.value("entered_childs").toString());
        ui->exit_time_label->setText(record.value("expected_exit_time").toDateTime().toString("dd.MM.yyyy    hh:mm:ss"));
    }
    else {
        ui->exit_label->setText("Выход #");
        ui->exit_time_label->setText("Время выхода: ");
        ui->exit_number_label->setText(record.value("exit_number").toString());
        ui->exit_time_label->setText(record.value("exit_time").toDateTime().toString("dd.MM.yyyy    hh:mm:ss"));
    }
    ui->ballance_label->setText(record.value("cash").toString());
    ui->comment_textEdit->setText(record.value("comment").toString());
}

void TransactionsFrame::computeTransactions(const int &id, const bool &active)
{
    QString queryString =
            active?QString("SELECT active_transactions.title as Действие, products.title AS Продукт, active_transactions.quantity as Количество, activity_point as Пункт, active_transactions.time as Время, active_transactions.total_price as Сумма "
                           "FROM active_transactions "
                           "LEFT JOIN products ON active_transactions.product_id=products.id "
                           "WHERE active_transactions.active_bracers_id=%1;").arg(id)
                 :QString("SELECT transactions_history.title as Действие, products.title AS Продукт, transactions_history.quantity as Количество, activity_point as Пункт, transactions_history.time as Время, transactions_history.total_price as Сумма "
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
        if(rec.value("Действие").toString()=="Регистрация" or rec.value("Действие").toString()=="Покупка")
            temp+=rec.value("Сумма").toDouble();
    }
    ui->total_transaction_label->setText(QString::number(temp, 'f', 2));
}

void TransactionsFrame::on_print_but_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableView->model()->rowCount();
    QSqlQueryModel *model = static_cast<QSqlQueryModel*>(ui->tableView->model());

    out <<  QString("<html><head><meta Content=\"Text/html; charset=Windows-1251\"></head><body>"
                    "<p align=center>--- OASIS ---<br>"
                    "<p>Браслет №: %1<br>"
                    "Количество детей: %2<br>"
                    "Вход: %3 [%4]<br>"
                    "Ожидаемый выход: %5<br>"
                    "Баланс: %6<br>"
                    "Транзакции:<br>"
                    "<ol>").arg(ui->bracer_number_label->text())
            .arg(ui->childs_label->text())
            .arg(ui->enter_time_label->text())
            .arg(ui->enter_number_label->text())
            .arg(ui->exit_time_label->text())
            .arg(ui->ballance_label->text());

    //    // data table
    for (int row = 0; row < rowCount; row++) {
        QSqlRecord rec = model->record(row);
        if(rec.value("Действие").toString()=="Покупка")
            out<<QString("<li><font size=2px>%1 [%2]</font><br>%3 x %4 - %5 UZS<li>")
                 .arg(rec.value("Время").toDateTime().toString("dd.MM.yyyy \\ hh:mm:ss"))
                 .arg(rec.value("Пункт").toString())
                 .arg(rec.value("Количество").toString())
                 .arg(rec.value("Продукт").toString())
                 .arg(rec.value("Сумма").toString());
        else
            out<<QString("<li><font size=2px>%1 [%2]</font><br>%3 - %4 UZS<li>")
                 .arg(rec.value("Время").toDateTime().toString("dd.MM.yyyy \\ hh:mm:ss"))
                 .arg(rec.value("Пункт").toString())
                 .arg(rec.value("Действие").toString())
                 .arg(rec.value("Сумма").toString());
    }
    out << QString("</ol><p align=right>---- Общий расход: %1 ----<p><font color=grey>powered by GSS.UZ</font></p></body></html>")
           .arg(ui->total_transaction_label->text());

    QPrinter bPrinter;
    bPrinter.setFullPage(true);
    bPrinter.setPageMargins(QMarginsF(2,2,2,2));

    QPrintDialog dialog(&bPrinter, NULL);
    if (dialog.exec() == QDialog::Accepted) {

        QTextDocument document;
        document.setPageSize(bPrinter.pageRect().size());
        document.setDocumentMargin(0);
        document.setHtml(strStream);
        document.print(&bPrinter);
    }
}
