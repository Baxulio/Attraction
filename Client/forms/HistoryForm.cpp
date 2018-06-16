#include "HistoryForm.h"
#include "ui_HistoryForm.h"

HistoryForm::HistoryForm(QWidget *parent):
    QWidget(parent),
    ui(new Ui::HistoryForm),
    transactions(new TransactionsFrame(parent)),
    bDb(DatabaseManager::instance()),
    tariffModel(new QSqlTableModel(this)),
    historyModel(new QSqlTableModel(this)),
    proxyModel(new ProxyModel(this))
{
    ui->setupUi(this);

    ui->transactions_frame->layout()->addWidget(transactions);

    ui->tariff_combo->setModel(tariffModel);

    historyModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    proxyModel->setSourceModel(historyModel);
    ui->model_table->setModel(proxyModel);

    connect(&bDb, &DatabaseManager::refresh, this, &HistoryForm::on_refresh);
    connect(ui->model_table->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)
        QSqlQuery query;
        if(!query.exec(QString("SELECT * "
                               "FROM (bracers_history "
                               "INNER JOIN tariff ON bracers_history.tariff_id=tariff.id) WHERE code=%1;").arg(historyModel->record(cur.row()).value("code").toUInt()))){
            bDb.debugQuery(query);
            return;
        }
        query.next();
        transactions->setHeaderData(query.record(),false);
        transactions->computeTransactions(historyModel->record(cur.row()).value("id").toInt(),false);
    });
    on_refresh();
}

HistoryForm::~HistoryForm()
{
    delete ui;
}

void HistoryForm::on_refresh()
{
    tariffModel->setTable("tariff");
    if(!tariffModel->select()){
        bDb.debugError(tariffModel->lastError());
        return ;
    }
    ui->tariff_combo->setModelColumn(1);

    historyModel->setTable("bracers_history");
    if(!historyModel->select()){
        bDb.debugError(historyModel->lastError());
        return ;
    }
    //    ui->people_label->setText(QString::number(historyModel->rowCount()));
    //    QSqlQuery query;
    //    if(!query.exec(QString("SELECT SUM(total_price) AS SUM "
    //                           "FROM active_transactions;"))){
    //        bDb.debugQuery(query);
    //        return;
    //    }
    //    query.next();
    //    if(!query.isValid())
    //        ui->total_price_label->setText("0");
    //    else ui->total_price_label->setText(query.value("SUM").toString());
}

void HistoryForm::on_back_but_clicked()
{
    emit back();
}

void HistoryForm::on_tariff_combo_currentIndexChanged(int index)
{
    proxyModel->setTariff_id(tariffModel->data(tariffModel->index(index,tariffModel->fieldIndex("id"))).toInt());
    proxyModel->invalidate();
}

void HistoryForm::on_filter_but_clicked()
{
    proxyModel->setBracerNumber(ui->bracer_number_spin->value());
    proxyModel->setEnterNumber(ui->enter_number_spin->value());
    proxyModel->setIn_time_from(ui->in_from_dateTime->dateTime());
    proxyModel->setIn_time_to(ui->in_to_dateTime->dateTime());
    proxyModel->invalidate();
}

void HistoryForm::on_clean_but_clicked()
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
