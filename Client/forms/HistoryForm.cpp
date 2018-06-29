#include "HistoryForm.h"
#include "ui_HistoryForm.h"

HistoryForm::HistoryForm(QWidget *parent):
    QWidget(parent),
    ui(new Ui::HistoryForm),
    transactions(new TransactionsFrame(parent)),
    bDb(DatabaseManager::instance()),
    historyModel(new QSqlTableModel(this)),
    proxyModel(new ProxyModel(this))
{
    ui->setupUi(this);

    ui->transactions_frame->layout()->addWidget(transactions);

    historyModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    proxyModel->setSourceModel(historyModel);
    ui->model_table->setModel(proxyModel);

    connect(&bDb, &DatabaseManager::refresh, this, &HistoryForm::on_refresh);
    connect(ui->model_table->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)
        QSqlQuery query;
        if(!query.exec(QString("SELECT * FROM bracers_history WHERE code=%1;")
                       .arg(proxyModel->data(proxyModel->index(cur.row(),historyModel->fieldIndex("code")),Qt::EditRole).toUInt()))){
            bDb.debugQuery(query);
            return;
        }
        query.next();
        transactions->setHeaderData(query.record(),false);
        transactions->computeTransactions(proxyModel->data(proxyModel->index(cur.row(),historyModel->fieldIndex("id")),Qt::EditRole).toUInt(),false);
    });
    on_refresh();
}

HistoryForm::~HistoryForm()
{
    delete ui;
}

void HistoryForm::on_refresh()
{
    historyModel->setTable("bracers_history");
    if(!historyModel->select()){
        bDb.debugError(historyModel->lastError());
        return ;
    }
}

void HistoryForm::on_back_but_clicked()
{
    emit back();
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

    proxyModel->invalidate();
}
