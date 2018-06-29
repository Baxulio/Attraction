#include "UniteForm.h"
#include "ui_UniteForm.h"

#include "QMessageBox"

#include "dialogs/WieagandReaderDialog.h"

UniteForm::UniteForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UniteForm),
    bDb(DatabaseManager::instance())
{
    ui->setupUi(this);
}

UniteForm::~UniteForm()
{
    delete ui;
}

void UniteForm::on_back_but_clicked()
{
    emit back();
}

void UniteForm::on_add_product_type_but_clicked()
{
    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code=dialog.getCode();
    if(!code)return;

    QSqlQuery query;
    if(!query.exec(QString("SELECT * FROM `Attraction`.`active_bracers` WHERE code=%1").arg(code))){
        bDb.debugQuery(query);
        return;
    }
    query.next();
    if(!query.isValid()){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Карта не зарегистрирована!"));
        return;
    }
    if(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(0,3)).toInt()==query.value("deposit_id")){
                QMessageBox::warning(this, "Неожиданная ситуация",
                                     QString("Добавляемый браслет уже имеет общий депозит с ведущим браслетом!"));
                return;
            }
    int countRow = ui->union_tableWidget->rowCount();
    for(int i=0; i<countRow; i++){
        if(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(i,0)).toUInt()==code){
            QMessageBox::warning(this, "Неожиданная ситуация",
                                 QString("Браслет уже добавлен в список!"));
            return;
        }
    }
    ui->union_tableWidget->insertRow(countRow);

    ui->union_tableWidget->model()->setData(ui->union_tableWidget->model()->index(countRow,0),code);
    ui->union_tableWidget->model()->setData(ui->union_tableWidget->model()->index(countRow,1),query.value("bracer_number").toInt());
    ui->union_tableWidget->model()->setData(ui->union_tableWidget->model()->index(countRow,3),query.value("deposit_id").toInt());

    if(!query.exec(QString("SELECT * FROM `Attraction`.`deposit` WHERE id=%1").arg(query.value("deposit_id").toInt()))){
        ui->union_tableWidget->removeRow(countRow);
        bDb.debugQuery(query);
        return;
    }
    query.next();
    ui->union_tableWidget->model()->setData(ui->union_tableWidget->model()->index(countRow,2),query.value("cash").toDouble());
    if(countRow==0){
        ui->union_tableWidget->setVerticalHeaderLabels(QStringList()<<"Ведущий депозит: ");
    }
}

void UniteForm::on_delete_product_type_but_clicked()
{
    ui->union_tableWidget->removeRow(ui->union_tableWidget->currentRow());

}

void UniteForm::on_unite_bracers_pushButton_clicked()
{
    int countRow = ui->union_tableWidget->rowCount();
    if(countRow<=1){
        QMessageBox::information(this, "Информация",
                                 QString("Необходимы 2 или более браслета для их объединения!"));
        return;
    }

    int depositId = ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(0,3)).toInt();
    QString query1 = QString("SELECT SUM(cash) AS SUM FROM deposit WHERE id = %1 ").arg(depositId);

    for(int i=1; i<countRow; i++){
        query1+=QString("OR id = %1").arg(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(i,3)).toInt());
    }

    QSqlQuery query;
    if(!query.exec(query1+";")){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Не удалось выполнить объединение! Попробуйте еще раз!"));
        bDb.debugQuery(query);
        return;
    }
    query.next();
    double total = query.value("SUM").toDouble();

    if(QMessageBox::question(this, "Вопрос",
                         QString("Результирующий депозит будет составлять %1. \nПродолжить операцию?").arg(total))
            !=QMessageBox::Yes)return;

    QString query2 = QString("UPDATE `Attraction`.`active_bracers` SET `deposit_id`=%1 WHERE `code`= %2 ")
            .arg(depositId)
            .arg(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(1,0)).toUInt());

    for(int i=2; i<countRow; i++){
        query2+=QString("OR `code`=%1 ").arg(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(i,0)).toUInt());
    }
    query2+=";";

    if(!query.exec(QString("CALL `unite_bracers`('%1', '%2')")
                   .arg(QString("UPDATE `Attraction`.`deposit` SET `cash`=%1 WHERE `id`=%2;")
                       .arg(total).arg(depositId)).arg(query2))){
        bDb.debugQuery(query);
        return;
    }

    ui->union_tableWidget->model()->removeRows(0,countRow);
    QMessageBox::information(this, "Успех",
                             QString("<font color='green'>Браслеты успешно объединены в один депозит!"));
}
