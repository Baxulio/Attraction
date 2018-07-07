#include "RegisterForm.h"
#include "ui_RegisterForm.h"

#include "QMessageBox"

#include <QSqlRecord>
#include <QDateTime>

#include "dialogs/WieagandReaderDialog.h"
#include <QDebug>

RegisterForm::RegisterForm(SettingsDialog &set, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterForm),
    bDb(DatabaseManager::instance()),
    bSetings(set)
{
    ui->setupUi(this);

    connect(&bDb, &DatabaseManager::refresh, this, &RegisterForm::on_refresh);
}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::on_refresh()
{
    if(!this->isVisible())return;
    ui->cash_doubleSpinBox->setMaximum(bDb.tariffModel->record(0).value("price_limit").toDouble());
}

void RegisterForm::on_back_pushButton_clicked()
{
    emit back();
}

void RegisterForm::on_register_bracer_but_clicked()
{
    if(!ui->bracer_number_spinBox->value()){
        ui->bracer_number_spinBox->setFocus();
        return;
    }

    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code=dialog.getCode();
    if(!code){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Неверный ввод!"));
        return;
    }

    QSqlQuery query;
    if(!query.exec(QString("SELECT * FROM active_bracers WHERE code=%1").arg(code))){
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
    if(!query.exec(QString("CALL `register`('%1', '%2', '%3','%4','%5','%6');")
                   .arg(ui->childs_spinBox->value())
                   .arg(ui->bracer_number_spinBox->value())
                   .arg(code)
                   .arg(ui->cash_doubleSpinBox->value())
                   .arg(ui->comment_textEdit->toPlainText())
                   .arg(bSetings.activityPointSettings().activityPointNumber)))
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
    if(!query.exec(QString("SELECT * FROM active_bracers WHERE code=%1").arg(code))){
        bDb.debugQuery(query);
        return;
    }
    query.next();
    if(!query.isValid()){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Браслет не зарегистрирован!"));
        return;
    }
    if(!query.value("enter_time").isNull()){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Данная операция невозможна: браслет уже зафиксирован при входе!"));
        return;
    }
    if(QMessageBox::question(this, "Вопрос",
                             QString("Вы действительно хотите сбросить данный браслет?"))
            !=QMessageBox::Yes)return;

    if(!query.exec(QString("DELETE FROM active_bracers WHERE  `code`=%1;")
                   .arg(code)))
        bDb.debugQuery(query);

    QMessageBox::information(this, "Успех",
                             QString("<font color='green'>Успешно!"));
}

void RegisterForm::on_bracer_number_spinBox_valueChanged(int arg1)
{
    ui->childs_spinBox->setValue(0);

    arg1?ui->price_label->setText(bDb.tariffModel->record(0).value("price").toString()):
         ui->price_label->setText("0");
}

void RegisterForm::on_childs_spinBox_valueChanged(int arg1)
{
    if(ui->bracer_number_spinBox->value())
        ui->price_label->setText(QString("%1").arg(bDb.tariffModel->record(0).value("price").toDouble()+bDb.tariffModel->record(1).value("price").toDouble()*arg1));
}
