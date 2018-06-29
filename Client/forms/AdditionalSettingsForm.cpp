#include "AdditionalSettingsForm.h"
#include "ui_AdditionalSettingsForm.h"

AdditionalSettingsForm::AdditionalSettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdditionalSettingsForm),
    bDb(DatabaseManager::instance())
{
    ui->setupUi(this);

    ui->tariffs_table->setModel(bDb.tariffModel);

    connect(&bDb, &DatabaseManager::refresh, this, &AdditionalSettingsForm::on_refresh);
    on_refresh();
}

AdditionalSettingsForm::~AdditionalSettingsForm()
{
    delete ui;
}

void AdditionalSettingsForm::on_refresh()
{
    ui->tariffs_table->hideColumn(bDb.tariffModel->fieldIndex("id"));

    bDb.tariffModel->setHeaderData(bDb.tariffModel->fieldIndex("title"),Qt::Horizontal,"Название");
    bDb.tariffModel->setHeaderData(bDb.tariffModel->fieldIndex("price"),Qt::Horizontal,"Цена");
    bDb.tariffModel->setHeaderData(bDb.tariffModel->fieldIndex("time_limit"),Qt::Horizontal,"Время [в минутах]");
}

void AdditionalSettingsForm::on_add_tariff_clicked()
{
//    tariffs_model->insertRow(tariffs_model->rowCount());
}

void AdditionalSettingsForm::on_delete_tariff_clicked()
{
//    tariffs_model->removeRow(ui->tariffs_table->currentIndex().row());
}

void AdditionalSettingsForm::on_tariffs_submit_clicked()
{
    if(!bDb.tariffModel->submitAll())
        bDb.debugError(bDb.tariffModel->lastError());
}

void AdditionalSettingsForm::on_tariffs_revert_clicked()
{
    bDb.tariffModel->revertAll();
}

void AdditionalSettingsForm::on_back_but_clicked()
{
    emit back();
}
