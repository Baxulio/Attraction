#include "DashboardForm.h"
#include "ui_DashboardForm.h"

#include <QItemSelectionModel>

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
    activeModel->setTable("active_bracers");
    if(!activeModel->select()){
        bDb.debugError(activeModel->lastError());
        return ;
    }
    QSqlQuery query;
    if(!query.exec(QString("SELECT SUM(total_price) AS SUM "
                           "FROM active_transactions;"))){
        bDb.debugQuery(query);
        return;
    }
    query.next();
    if(!query.isValid())
        ui->total_price_label->setText("0");
    else ui->total_price_label->setText(query.value("SUM").toString());

    int rows = activeModel->rowCount();
    int children = 0;

    QSqlRecord rec;
    for(int i=0; i<rows; i++){
        rec = activeModel->record(i);
        children += rec.value("childs").toInt();
    }
    ui->children_label->setText(QString::number(children));
    ui->people_label->setText(QString::number(rows));
}

void DashboardForm::on_back_but_clicked()
{
    emit back();
}

void DashboardForm::on_clean_but_clicked()
{
    ui->bracer_number_spin->setValue(0);
    proxyModel->setBracerNumber(0);

    ui->enter_number_spin->setValue(0);
    proxyModel->setEnterNumber(0);

    ui->childs_spin->setValue(0);
    proxyModel->setChildsNumber(0);

    ui->in_from_dateTime->setDateTime(QDateTime());
    proxyModel->setIn_time_from(QDateTime());

    ui->in_to_dateTime->setDateTime(QDateTime());
    proxyModel->setIn_time_to(QDateTime());

    proxyModel->invalidate();
}

void DashboardForm::on_filter_but_clicked()
{
    proxyModel->setBracerNumber(ui->bracer_number_spin->value());
    proxyModel->setEnterNumber(ui->enter_number_spin->value());
    proxyModel->setChildsNumber(ui->childs_spin->value());
    proxyModel->setIn_time_from(ui->in_from_dateTime->dateTime());
    proxyModel->setIn_time_to(ui->in_to_dateTime->dateTime());
    proxyModel->invalidate();
}

void DashboardForm::on_time_out_but_clicked(bool checked)
{
    proxyModel->setTimeOutEnabled(checked);
}

void DashboardForm::on_warning_but_clicked(bool checked)
{
    proxyModel->setWarningEnabled(checked);
}
