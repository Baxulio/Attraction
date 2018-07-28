#include "AdditionalSettingsForm.h"
#include "ui_AdditionalSettingsForm.h"

#include <QMessageBox>

#include "dialogs/WieagandReaderDialog.h"

AdditionalSettingsForm::AdditionalSettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdditionalSettingsForm),
    bDb(DatabaseManager::instance())
{
    ui->setupUi(this);

    ui->tariffs_table->setModel(bDb.tariffModel);
    ui->staff_table->setModel(&staffModel);

    connect(&bDb, &DatabaseManager::refresh, this, &AdditionalSettingsForm::on_refresh);
    on_refresh();
}

AdditionalSettingsForm::~AdditionalSettingsForm()
{
    delete ui;
}

void AdditionalSettingsForm::on_refresh()
{
    if(!this->isVisible())return;

    staffModel.setTable("staff");
    if(!staffModel.select()){
        bDb.debugError(staffModel.lastError());
        return ;
    }

    ui->staff_table->hideColumn(staffModel.fieldIndex("id_code"));
    staffModel.setHeaderData(staffModel.fieldIndex("name"), Qt::Horizontal, "Имя",Qt::DisplayRole);
    staffModel.setHeaderData(staffModel.fieldIndex("occupation"), Qt::Horizontal, "Должность",Qt::DisplayRole);

    ui->tariffs_table->hideColumn(bDb.tariffModel->fieldIndex("id"));

    bDb.tariffModel->setHeaderData(bDb.tariffModel->fieldIndex("title"),Qt::Horizontal,"Название",Qt::DisplayRole);
    bDb.tariffModel->setHeaderData(bDb.tariffModel->fieldIndex("price"),Qt::Horizontal,"Цена",Qt::DisplayRole);
    bDb.tariffModel->setHeaderData(bDb.tariffModel->fieldIndex("time_limit"),Qt::Horizontal,"Время [в минутах]",Qt::DisplayRole);
    bDb.tariffModel->setHeaderData(bDb.tariffModel->fieldIndex("price_limit"),Qt::Horizontal,"Ограничение баланса [в UZS]",Qt::DisplayRole);
}

void AdditionalSettingsForm::on_add_tariff_clicked()
{
        bDb.tariffModel->insertRow(bDb.tariffModel->rowCount());
}

void AdditionalSettingsForm::on_delete_tariff_clicked()
{
        bDb.tariffModel->removeRow(ui->tariffs_table->currentIndex().row());
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

void AdditionalSettingsForm::on_add_stuff_but_clicked()
{
    quint32 code;
    if(!getCode(code))return;

    int row = staffModel.rowCount();
    staffModel.insertRow(row);
    staffModel.setData(staffModel.index(row,staffModel.fieldIndex("id_code")),code);
}

void AdditionalSettingsForm::on_delete_stuff_but_clicked()
{
    staffModel.removeRow(ui->staff_table->currentIndex().row());
}

void AdditionalSettingsForm::on_staff_submit_clicked()
{
    if(!staffModel.submitAll())
        bDb.debugError(staffModel.lastError());
}

void AdditionalSettingsForm::on_staff_revert_clicked()
{
    staffModel.revertAll();
}

void AdditionalSettingsForm::on_chang_bracer_but_clicked()
{
    quint32 code;
    if(getCode(code))
        staffModel.setData(staffModel.index(ui->staff_table->currentIndex().row(),staffModel.fieldIndex("id_code")),code);
}

bool AdditionalSettingsForm::getCode(quint32 &code)
{
    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return false;
    }
    code=dialog.getCode();
    if(!code){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Неверный ввод!"));
        return false;
    }
    return true;
}
