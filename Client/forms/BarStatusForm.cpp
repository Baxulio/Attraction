#include "BarStatusForm.h"
#include "ui_BarStatusForm.h"

#include <QMessageBox>
#include "dialogs/WieagandReaderDialog.h"

#include <QDateTime>

#include <QTcpSocket>

BarStatusForm::BarStatusForm(SettingsDialog &set, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarStatusForm),
    transactionsFrame(new TransactionsFrame(parent)),
    bDb(DatabaseManager::instance()),
    bSetings(set)
{
    ui->setupUi(this);
    ui->transactions_frame->layout()->addWidget(transactionsFrame);
    ui->lineEdit->setFocus();
}

BarStatusForm::~BarStatusForm()
{
    delete ui;
}

bool BarStatusForm::retrieve_info(const quint32 &code)
{
    QSqlQuery query;
    if(!query.exec(QString("SELECT active_bracers.id, active_bracers.code, active_bracers.bracer_number, "
                           "active_bracers.enter_time, active_bracers.expected_exit_time, active_bracers.enter_number, active_bracers.entered_childs, active_bracers.deposit_id,"
                           "active_bracers.childs, active_bracers.comment, deposit.cash, SYSDATE() as cur_date_time, tariff.title, tariff.price_limit, tariff.price, tariff.time_limit "
                           "FROM ((active_bracers "
                           "INNER JOIN deposit ON active_bracers.deposit_id=deposit.id) "
                           "INNER JOIN tariff ON active_bracers.tariff_id = tariff.id) "
                           "WHERE code=%1;").arg(code))){
        bDb.debugQuery(query);
        return false;
    }
    query.next();

    if(!query.isValid()){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Карта не зарегистрирована!"));

        return false;
    }

    QDateTime enterTime = query.value("enter_time").toDateTime();

    quint64 timeLimit= enterTime.secsTo(query.value("expected_exit_time").toDateTime())/60;
    quint64 elapsed = enterTime.secsTo(query.value("cur_date_time").toDateTime())/60;

    if(elapsed>timeLimit){
        //here goes penalty
        double cache = query.value("cash").toDouble();
        elapsed-=timeLimit;
        int minutes = int(elapsed%60==0?elapsed/60:1+elapsed/60)*60;

        double penalty = minutes*(query.value("price").toDouble()/query.value("time_limit").toInt());
        if(!query.exec(QString("CALL penalty(%1,%2,%3,%4,%5,%6);")
                       .arg(minutes)
                       .arg(query.value("id").toInt())
                       .arg(cache-penalty)
                       .arg(query.value("deposit_id").toInt())
                       .arg(bSetings.activityPointSettings().activityPointNumber)
                       .arg(penalty))){
            bDb.debugQuery(query);
            return false;
        }
        return retrieve_info(code);
    }

    currentRecord = query.record();

    transactionsFrame->transactionModel.clear();
    transactionsFrame->setHeaderData(currentRecord,true);

    return true;
}

void BarStatusForm::on_back_but_clicked()
{
    emit back();
}

void BarStatusForm::on_lineEdit_returnPressed()
{
    if(retrieve_info(ui->lineEdit->text().toUInt()))
        transactionsFrame->computeTransactions(currentRecord.value("id").toInt(),true);
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
}
