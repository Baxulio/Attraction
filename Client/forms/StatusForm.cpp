#include "StatusForm.h"
#include "ui_StatusForm.h"

#include <QMessageBox>
#include "dialogs/WieagandReaderDialog.h"
#include "dialogs/PaymentMethodDialog.h"

#include <QDateTime>

#include <QTcpSocket>

StatusForm::StatusForm(SettingsDialog &set, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusForm),
    transactionsFrame(new TransactionsFrame(parent)),
    bDb(DatabaseManager::instance()),
    bSetings(set)
{
    ui->setupUi(this);
    ui->transactions_frame->layout()->addWidget(transactionsFrame);
}

StatusForm::~StatusForm()
{
    delete ui;
}

bool StatusForm::retrieve_info(const quint32 &code)
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
        ui->add_ballance_but->setEnabled(false);
        ui->pay_but->setEnabled(false);
        ui->return_money_but->setEnabled(false);
        ui->allow_but->setEnabled(false);

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

    ui->add_ballance_but->setEnabled(true);

    double ballance = currentRecord.value("cash").toDouble();
    if(ballance==0){
        ui->pay_but->setEnabled(false);
        ui->return_money_but->setEnabled(false);
        ui->allow_but->setEnabled(true);
    }
    else if(ballance<0){
        ui->pay_but->setEnabled(true);
        ui->return_money_but->setEnabled(false);
        ui->allow_but->setEnabled(false);
    }
    else {
        ui->pay_but->setEnabled(false);
        ui->return_money_but->setEnabled(true);
        ui->allow_but->setEnabled(false);
    }

    ui->cash_doubleSpinBox->setMaximum(currentRecord.value("price_limit").toDouble()-ballance);

    return true;
}

void StatusForm::on_back_but_clicked()
{
    emit back();
}

void StatusForm::on_retrieve_info_but_clicked()
{
    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    if(retrieve_info(dialog.getCode()))
        transactionsFrame->computeTransactions(currentRecord.value("id").toInt(),true);

    transactionsFrame->on_print_but_clicked();
}

void StatusForm::on_allow_but_clicked()
{
    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }

    quint32 code = dialog.getCode();
    if(!retrieve_info(code) or !ui->allow_but->isEnabled()){
        QMessageBox::warning(this, "Неудалось открыть турникет",
                             QString("Для выяснения причины перепроверьте браслет!"));
        return;
    }

    if(currentRecord.value("enter_time").isNull()){
        QMessageBox::warning(this, "Неудалось открыть турникет",
                             QString("Браслет не зафиксирован при входе!"));
        return;
    }

    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(bSetings.bareerSettings().host, bSetings.bareerSettings().port);
    if(!socket->waitForConnected(3000)){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Не удалось подключиться к турникету!"));
        return;
    }
    /////here is code for opening gate
    ///
    if(socket->write(QByteArray::number(code))<0)
        qDebug()<<"fuka";

    socket->waitForBytesWritten(5000);
    socket->deleteLater();
    ///
    ///
    if(retrieve_info(code))
        transactionsFrame->computeTransactions(currentRecord.value("id").toInt(),true);
}

void StatusForm::on_add_ballance_but_clicked()
{
    if(ui->cash_doubleSpinBox->value()<=0){
        ui->cash_doubleSpinBox->setFocus();
        return;
    }

    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code = dialog.getCode();
    if(!retrieve_info(code) or !ui->add_ballance_but->isEnabled())
        return;

    PaymentMethodDialog paymentMethodDialog(QString("Пополняемая сумма: %1\nВы точно хотите сделать это?").arg(ui->cash_doubleSpinBox->value()),this);
    if(paymentMethodDialog.exec() != QDialog::Accepted){
        return;
    }

    QSqlQuery query;
    if(!query.exec(QString("CALL `add_cash`(%1, %2, %3, %4, %5);")
                   .arg(ui->cash_doubleSpinBox->value())
                   .arg(currentRecord.value("id").toInt())
                   .arg(currentRecord.value("deposit_id").toInt())
                   .arg(bSetings.activityPointSettings().activityPointNumber)
                   .arg(paymentMethodDialog.isTerminal()))){
        bDb.debugQuery(query);
        return;
    }
    if(retrieve_info(code))
        transactionsFrame->computeTransactions(currentRecord.value("id").toInt(),true);
    ui->cash_doubleSpinBox->clear();
}

void StatusForm::on_pay_but_clicked()
{
    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code = dialog.getCode();

    if(!retrieve_info(code) or !ui->pay_but->isEnabled())
        return;

    PaymentMethodDialog paymentMethodDialog(QString("Оплачиваемая сумма: %1\nВы точно хотите сделать это?").arg(-currentRecord.value("cash").toDouble()),this);
    if(paymentMethodDialog.exec() != QDialog::Accepted){
        return;
    }

    QSqlQuery query;
    if(!query.exec(QString("CALL `return_debt`(%1, %2, %3, %4, %5);")
                   .arg(currentRecord.value("deposit_id").toInt())
                   .arg(currentRecord.value("id").toInt())
                   .arg(-currentRecord.value("cash").toDouble())
                   .arg(bSetings.activityPointSettings().activityPointNumber)
                   .arg(paymentMethodDialog.isTerminal()))){
        bDb.debugQuery(query);
        return;
    }

    if(retrieve_info(code))
        transactionsFrame->computeTransactions(currentRecord.value("id").toInt(),true);
}

void StatusForm::on_return_money_but_clicked()
{
    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code = dialog.getCode();

    if(!retrieve_info(code) or !ui->return_money_but->isEnabled())
        return;

    if (QMessageBox::question(this, "Появился вопрос",
                              QString("Возвращаемая сумма: %1\nВы точно хотите сделать это?").arg(currentRecord.value("cash").toDouble()))
            !=QMessageBox::Yes)
        return;

    QSqlQuery query;
    if(!query.exec(QString("CALL `return_remainder`(%1, %2, %3, %4);")
                   .arg(currentRecord.value("deposit_id").toInt())
                   .arg(currentRecord.value("id").toInt())
                   .arg(currentRecord.value("cash").toDouble())
                   .arg(bSetings.activityPointSettings().activityPointNumber))){
        bDb.debugQuery(query);
        return;
    }
    if(retrieve_info(code))
        transactionsFrame->computeTransactions(currentRecord.value("id").toInt(),true);

}
