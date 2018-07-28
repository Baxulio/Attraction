#include "DashboardForm.h"
#include "ui_DashboardForm.h"

#include <QItemSelectionModel>
#include "dialogs/BracersFilterDialog.h"

#include <QDebug>

DashboardForm::DashboardForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashboardForm),
    transactions(new TransactionsFrame(parent)),
    bDb(DatabaseManager::instance()),
    activeModel(new QSqlTableModel(this)),
    proxyModel(new ProxyModel(this))
{
    ui->setupUi(this);
    ui->transactions_frame->layout()->addWidget(transactions);

    activeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    proxyModel->setSourceModel(activeModel);
    ui->model_table->setModel(proxyModel);

    connect(&bDb, &DatabaseManager::refresh, this, &DashboardForm::on_refresh);
    connect(ui->model_table->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)
        QSqlQuery query;
        if(!query.exec(QString("SELECT active_bracers.id, active_bracers.code, active_bracers.bracer_number, active_bracers.enter_time, active_bracers.enter_number, active_bracers.deposit_id, deposit.cash,active_bracers.childs "
                               "FROM (active_bracers "
                               "INNER JOIN deposit ON active_bracers.deposit_id=deposit.id) "
                               "WHERE code=%1;").arg(activeModel->record(cur.row()).value("code").toUInt()))){
            bDb.debugQuery(query);
            return;
        }
        query.next();
        transactions->setHeaderData(query.record(),true);
        transactions->computeTransactions(query.record().value("id").toInt(),true);
    });
    on_refresh();
}

DashboardForm::~DashboardForm()
{
    delete ui;
}

void DashboardForm::on_refresh()
{
    if(!this->isVisible())return;

    activeModel->setTable("active_bracers");
    if(!activeModel->select()){
        bDb.debugError(activeModel->lastError());
        return ;
    }

    ui->model_table->hideColumn(activeModel->fieldIndex("id"));
    ui->model_table->hideColumn(activeModel->fieldIndex("code"));
    ui->model_table->hideColumn(activeModel->fieldIndex("deposit_id"));
    ui->model_table->hideColumn(activeModel->fieldIndex("cash"));
    ui->model_table->hideColumn(activeModel->fieldIndex("childs"));
    ui->model_table->hideColumn(activeModel->fieldIndex("entered_childs"));
    ui->model_table->hideColumn(activeModel->fieldIndex("comment"));
    ui->model_table->hideColumn(activeModel->fieldIndex("tariff_id"));

    activeModel->setHeaderData(activeModel->fieldIndex("bracer_number"),Qt::Horizontal,"Номер браслета");
    activeModel->setHeaderData(activeModel->fieldIndex("enter_time"),Qt::Horizontal,"Время входа");
    activeModel->setHeaderData(activeModel->fieldIndex("enter_number"),Qt::Horizontal,"Вход №");
    activeModel->setHeaderData(activeModel->fieldIndex("expected_exit_time"),Qt::Horizontal,"Ожидаемый выход");

    updateInfo();
}

void DashboardForm::on_back_but_clicked()
{
    emit back();
}

void DashboardForm::on_clean_but_clicked()
{
    proxyModel->setTariffId(0);
    proxyModel->setBracerNumber(0);
    proxyModel->setEnterNumber(0);
    proxyModel->setChildsNumber(0);
    proxyModel->setIn_time_from(QDateTime());
    proxyModel->setIn_time_to(QDateTime());
    proxyModel->invalidate();
    updateInfo();
}

void DashboardForm::on_filter_but_clicked()
{
    BracersFilterDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }

    proxyModel->setTariffId(dialog.tariff());
    proxyModel->setBracerNumber(dialog.bracerNumber());
    proxyModel->setEnterNumber(dialog.enterNumber());
    proxyModel->setChildsNumber(dialog.childs());
    proxyModel->setIn_time_from(dialog.enterTimeFrom());
    proxyModel->setIn_time_to(dialog.enterTimeTo());

    proxyModel->invalidate();
    updateInfo();
}

void DashboardForm::on_time_out_but_clicked(bool checked)
{
    proxyModel->setTimeOutEnabled(checked);
}

void DashboardForm::on_warning_but_clicked(bool checked)
{
    proxyModel->setWarningEnabled(checked);
}

void DashboardForm::updateInfo()
{
    int rows = proxyModel->rowCount();
    int tariff_rows = bDb.tariffModel->rowCount();
    int tariff_col = activeModel->fieldIndex("tariff_id");

    QString temp;
    for(int i=0; i<tariff_rows; i++){
        int tariff_count=0;
        for(int j=0; j<rows; j++){
            if(proxyModel->data(proxyModel->index(j,tariff_col),Qt::EditRole).toInt()==bDb.tariffModel->record(i).value("id").toInt())
                tariff_count++;
        }
        temp += QString("<span style=\" font-size:14pt; color:#55aaff;\">%1 </span>"
                        "<span style=\" color:#646464;\">%2  </span>")
                .arg(tariff_count)
                .arg(bDb.tariffModel->record(i).value("title").toString());
    }

    int childs = 0;
    int childs_col = activeModel->fieldIndex("childs");
    for(int j=0; j<rows; j++){
        childs+=proxyModel->data(proxyModel->index(j,childs_col), Qt::EditRole).toInt();
    }
    if(childs)
        temp += QString("<span style=\" font-size:14pt; color:#55aaff;\">+%1 </span>"
                    "<span style=\" color:#646464;\">Детей без браслета</span></p>")
            .arg(childs);

    ui->info_label->setText(temp);
}
