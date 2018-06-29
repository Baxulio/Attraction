#include "StatusForm.h"
#include "ui_StatusForm.h"

#include <QMessageBox>
#include "dialogs/WieagandReaderDialog.h"

StatusForm::StatusForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusForm),
    transactionsFrame(new TransactionsFrame(parent)),
    bDb(DatabaseManager::instance())
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
    if(!query.exec(QString("SELECT active_bracers.id, active_bracers.code, active_bracers.bracer_number, active_bracers.enter_time, active_bracers.enter_number, active_bracers.deposit_id,active_bracers.childs, deposit.cash "
                           "FROM (active_bracers "
                           "INNER JOIN deposit ON active_bracers.deposit_id=deposit.id) "
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
    currentRecord = query.record();

    transactionsFrame->transactionModel.clear();
    transactionsFrame->setHeaderData(currentRecord,true);

    double ballance = currentRecord.value("cash").toDouble();
    ballance<0?ui->return_debt_but->setEnabled(true):
               ui->return_debt_but->setEnabled(false);

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
}

void StatusForm::on_return_debt_but_clicked()
{
    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code = dialog.getCode();

    if(!retrieve_info(code))
        return;

    if (QMessageBox::question(this, "Появился вопрос",
                              QString("Оплачиваемая сумма: %1\nВы точно хотите сделать это?").arg(currentRecord.value("cash").toDouble()))
            !=QMessageBox::Yes)
        return;

    QSqlQuery query;
    if(!query.exec(QString("CALL `return_debt`(%1, %2, %3);")
                   .arg(currentRecord.value("deposit_id").toInt())
                   .arg(currentRecord.value("id").toInt())
                   .arg(currentRecord.value("cash").toDouble()))){
        bDb.debugQuery(query);
        return;
    }
    if(retrieve_info(code))
        transactionsFrame->computeTransactions(currentRecord.value("id").toInt(),true);
}
