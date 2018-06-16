#include "DashboardForm.h"
#include "ui_DashboardForm.h"
#include <QItemSelectionModel>

#include <QDebug>
DashboardForm::DashboardForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashboardForm),
    transactions(new TransactionsFrame(parent)),
    bDb(DatabaseManager::instance()),
    tariffModel(new QSqlTableModel(this)),
    activeModel(new QSqlTableModel(this)),
    proxyModel(new ProxyModel(this))
{
    ui->setupUi(this);
    ui->transactions_frame->layout()->addWidget(transactions);

    ui->tariff_combo->setModel(tariffModel);

    activeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    proxyModel->setSourceModel(activeModel);
    ui->model_table->setModel(proxyModel);

    connect(&bDb, &DatabaseManager::refresh, this, &DashboardForm::on_refresh);
    connect(ui->model_table->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)
        QSqlQuery query;
        if(!query.exec(QString("SELECT active_bracers.id, active_bracers.code, active_bracers.bracer_number, active_bracers.enter_time, active_bracers.enter_number, active_bracers.deposit_id, deposit.cash,tariff.title "
                               "FROM ((active_bracers "
                               "INNER JOIN deposit ON active_bracers.deposit_id=deposit.id) "
                               "INNER JOIN tariff ON active_bracers.tariff_id=tariff.id) WHERE code=%1;").arg(activeModel->record(cur.row()).value("code").toUInt()))){
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
    tariffModel->setTable("tariff");
    if(!tariffModel->select()){
        bDb.debugError(tariffModel->lastError());
        return ;
    }
    ui->tariff_combo->setModelColumn(1);

    activeModel->setTable("active_bracers");
    if(!activeModel->select()){
        bDb.debugError(activeModel->lastError());
        return ;
    }
    ui->people_label->setText(QString::number(activeModel->rowCount()));
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

    ui->in_from_dateTime->setDateTime(QDateTime());
    proxyModel->setIn_time_from(QDateTime());

    ui->in_to_dateTime->setDateTime(QDateTime());
    proxyModel->setIn_time_to(QDateTime());

    ui->tariff_combo->setCurrentIndex(-1);
    proxyModel->setTariff_id(0);

    proxyModel->invalidate();
}

void DashboardForm::on_filter_but_clicked()
{
    proxyModel->setBracerNumber(ui->bracer_number_spin->value());
    proxyModel->setEnterNumber(ui->enter_number_spin->value());
    proxyModel->setIn_time_from(ui->in_from_dateTime->dateTime());
    proxyModel->setIn_time_to(ui->in_to_dateTime->dateTime());
    proxyModel->invalidate();
}

void DashboardForm::on_tariff_combo_currentIndexChanged(int index)
{
    proxyModel->setTariff_id(tariffModel->data(tariffModel->index(index,tariffModel->fieldIndex("id"))).toInt());
    proxyModel->invalidate();
}
