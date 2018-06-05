#include "RegisterForm.h"
#include "ui_RegisterForm.h"

#include "QMessageBox"
#include "QDateTime"
#include "QComboBox"

#include <QSqlRecord>

#include "dialogs/WieagandReaderDialog.h"
#include <QDebug>

RegisterForm::RegisterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterForm),
    bDb(DatabaseManager::instance()),
    tariffModel(new QSqlTableModel(this))
{
    ui->setupUi(this);
    ui->tariff_comboBox->setModel(tariffModel);
    connect(&bDb, &DatabaseManager::refresh, this, &RegisterForm::on_refresh);
    on_refresh();
}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::on_back_pushButton_clicked()
{
    emit back();
}

void RegisterForm::on_refresh()
{
    tariffModel->setTable("tariff");
    if(!tariffModel->select()){
        bDb.debugError(tariffModel->lastError());
        return ;
    }
    ui->tariff_comboBox->setModelColumn(1);
}

void RegisterForm::on_register_bracer_but_clicked()
{
    int currentTariffIndx = ui->tariff_comboBox->currentIndex();

    if(ui->bracer_number_spinBox->value()==0){
        ui->bracer_number_spinBox->setFocus();
        return;
    }
    else if(currentTariffIndx<0){
        ui->tariff_comboBox->setFocus();
        return;
    }

    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code=dialog.getCode();
    if(code==0){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Неверный ввод!"));
        return;
    }
    int b_tariff_id = tariffModel->record(currentTariffIndx).value("id").toInt();

    QSqlQuery query;
    if(!query.exec(QString("SELECT * FROM `Attraction`.`active_bracers` WHERE code=%1").arg(code))){
        bDb.debugQuery(query);
        return;
    }
    query.next();
    if(query.isValid()){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Браслет уже зарегистрирован!\n"
                                     "Номер браслета: %1\n"
                                     "Время входа: %2\n"
                                     "Номер входа: %3\n"
                                     ).arg(query.value("bracer_number").toString())
                             .arg(query.value("enter_time").toDateTime().toString())
                             .arg(query.value("enter_number").toInt()));
        return;
    }
    if(!query.exec(QString("CALL `register`('%1', '%2', '%3', '%4', '%5');")
                   .arg(ui->cash_doubleSpinBox->value())
                   .arg(ui->bracer_number_spinBox->value())
                   .arg(b_tariff_id)
                   .arg(code)
                   .arg(tariffModel->record(currentTariffIndx).value("price").toDouble())))
        bDb.debugQuery(query);

    QMessageBox::information(this, "Успех",
                             QString("<font color='green'>Успешно зарегистрирован!"));
}

void RegisterForm::on_drop_bracer_but_clicked()
{
    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code=dialog.getCode();

    QSqlQuery query;
    if(!query.exec(QString("SELECT * FROM `Attraction`.`active_bracers` WHERE code=%1").arg(code))){
        bDb.debugQuery(query);
        return;
    }
    query.next();
    if(!query.isValid()){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Браслет не зарегистрирован!"));
        return;
    }
    if(QMessageBox::question(this, "Вопрос",
                             QString("Вы действительно хотите удалить данный браслет?"))
            !=QMessageBox::Yes)return;

    if(!query.exec(QString("DELETE FROM `Attraction`.`active_bracers` WHERE  `code`=%1;")
                   .arg(code)))
        bDb.debugQuery(query);

    QMessageBox::information(this, "Успех",
                             QString("<font color='green'>Успешно удален!"));
}

void RegisterForm::on_tariff_comboBox_currentIndexChanged(int index)
{
    int time = tariffModel->data(tariffModel->index(index,3)).toInt();
    ui->hours_label->setText(QString::number(time/60));
    ui->minutes_label->setText(QString::number(time%60));
    ui->price_label->setText(tariffModel->data(tariffModel->index(index,2)).toString());
}
