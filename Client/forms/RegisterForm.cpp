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

void RegisterForm::updatePriceLabel()
{
    double price = bDb.tariffModel->record(ui->tariff_comboBox->currentIndex()).value("price").toDouble()+
            ui->childs_spinBox->value()*bDb.tariffModel->record(ui->tariff_comboBox->findText("Детский")).value("price").toDouble()+
            ui->cash_doubleSpinBox->value();

    ui->price_label->setText(QString::number(price, 'f', 2));
}

void RegisterForm::on_refresh()
{
    if(!this->isVisible())return;

    ui->tariff_comboBox->setModel(bDb.tariffModel);
    ui->tariff_comboBox->setModelColumn(1);
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
    if(!query.exec(QString("CALL `register`('%1', '%2', '%3','%4','%5','%6' ,'%7', %8);")
                   .arg(ui->childs_spinBox->value())
                   .arg(ui->bracer_number_spinBox->value())
                   .arg(code)
                   .arg(ui->cash_doubleSpinBox->value())
                   .arg(ui->comment_textEdit->toPlainText())
                   .arg(bSetings.activityPointSettings().activityPointNumber)
                   .arg(ui->terminal_radio->isChecked()?"Терминал":"Наличные")
                   .arg(bDb.tariffModel->record(ui->tariff_comboBox->currentIndex()).value("id").toInt())))
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
    Q_UNUSED(arg1)
    updatePriceLabel();
}

void RegisterForm::on_childs_spinBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    updatePriceLabel();
}

void RegisterForm::on_tariff_comboBox_currentIndexChanged(int index)
{
    ui->cash_doubleSpinBox->setMaximum(bDb.tariffModel->record(index).value("price_limit").toDouble());
    updatePriceLabel();
}

void RegisterForm::on_resetFields_but_clicked()
{
    ui->tariff_comboBox->setCurrentIndex(0);
    ui->bracer_number_spinBox->setValue(0);
    ui->childs_spinBox->setValue(0);
    ui->cash_doubleSpinBox->setValue(0);
    ui->real_radio->setChecked(true);
    ui->comment_textEdit->clear();
}

void RegisterForm::on_cash_doubleSpinBox_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    updatePriceLabel();
}
