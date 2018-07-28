#include "BracersFilterDialog.h"
#include "ui_BracersFilterDialog.h"

#include <QSqlRecord>

BracersFilterDialog::BracersFilterDialog(bool b, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BracersFilterDialog),
    bDb(DatabaseManager::instance())
{
    ui->setupUi(this);
    if(b){
        ui->exit_num_spin->setEnabled(false);
        ui->exit_group->setEnabled(false);
    }
    ui->tariff_comboBox->setModel(bDb.tariffModel);
    ui->tariff_comboBox->setModelColumn(1);
}

BracersFilterDialog::~BracersFilterDialog()
{
    delete ui;
}

int BracersFilterDialog::tariff()
{
    return bDb.tariffModel->record(ui->tariff_comboBox->currentIndex()).value("id").toInt();
}

int BracersFilterDialog::bracerNumber()
{
    return ui->bracer_number_spin->value();
}

int BracersFilterDialog::enterNumber()
{
    return ui->enter_number_spin->value();
}

int BracersFilterDialog::childs()
{
    return ui->childs_spin->value();
}

QDateTime BracersFilterDialog::enterTimeFrom()
{
    return ui->in_from_dateTime->dateTime();
}

QDateTime BracersFilterDialog::enterTimeTo()
{
    return ui->in_to_dateTime->dateTime();
}

int BracersFilterDialog::exitNumber()
{
    return ui->exit_num_spin->value();
}

QDateTime BracersFilterDialog::exitTimeFrom()
{
    return ui->out_from_dateTime->dateTime();
}

QDateTime BracersFilterDialog::exitTimeTo()
{
    return ui->out_to_dateTime->dateTime();
}

void BracersFilterDialog::on_today_for_enter_but_clicked()
{
    ui->in_from_dateTime->setDateTime(QDateTime(QDate::currentDate(), QTime(0,0)));
    ui->in_to_dateTime->setDateTime(QDateTime(QDate::currentDate(), QTime(23,59)));
}

void BracersFilterDialog::on_today_for_exit_but_clicked()
{
    ui->out_from_dateTime->setDateTime(QDateTime(QDate::currentDate(), QTime(0,0)));
    ui->out_to_dateTime->setDateTime(QDateTime(QDate::currentDate(), QTime(23,59)));
}
