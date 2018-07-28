#include "HistoryForm.h"
#include "ui_HistoryForm.h"

#include "dialogs/BracersFilterDialog.h"

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
    if(!this->isVisible())return;

    historyModel->setTable("bracers_history");
    if(!historyModel->select()){
        bDb.debugError(historyModel->lastError());
        return ;
    }
    ui->model_table->hideColumn(historyModel->fieldIndex("id"));
    ui->model_table->hideColumn(historyModel->fieldIndex("code"));
    ui->model_table->hideColumn(historyModel->fieldIndex("childs"));
    ui->model_table->hideColumn(historyModel->fieldIndex("entered_childs"));
    ui->model_table->hideColumn(historyModel->fieldIndex("comment"));
    ui->model_table->hideColumn(historyModel->fieldIndex("tariff_id"));

    historyModel->setHeaderData(historyModel->fieldIndex("bracer_number"),Qt::Horizontal,"Номер браслета");
    historyModel->setHeaderData(historyModel->fieldIndex("enter_time"),Qt::Horizontal,"Время входа");
    historyModel->setHeaderData(historyModel->fieldIndex("enter_number"),Qt::Horizontal,"Вход №");
    historyModel->setHeaderData(historyModel->fieldIndex("exit_time"),Qt::Horizontal,"Время выхода");
    historyModel->setHeaderData(historyModel->fieldIndex("exit_number"),Qt::Horizontal,"Выход №");

    updateInfo();
}

void HistoryForm::on_back_but_clicked()
{
    emit back();
}

void HistoryForm::on_filter_but_clicked()
{
    BracersFilterDialog dialog(false, this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }

    proxyModel->setTariffId(dialog.tariff());
    proxyModel->setBracerNumber(dialog.bracerNumber());
    proxyModel->setEnterNumber(dialog.enterNumber());
    proxyModel->setChildsNumber(dialog.childs());
    proxyModel->setIn_time_from(dialog.enterTimeFrom());
    proxyModel->setIn_time_to(dialog.enterTimeTo());

    proxyModel->setExitNumber(dialog.exitNumber());
    proxyModel->setOut_time_from(dialog.exitTimeFrom());
    proxyModel->setOut_time_to(dialog.exitTimeTo());

    proxyModel->invalidate();
    updateInfo();
}

void HistoryForm::on_clean_but_clicked()
{
    proxyModel->setTariffId(0);
    proxyModel->setBracerNumber(0);
    proxyModel->setEnterNumber(0);
    proxyModel->setChildsNumber(0);
    proxyModel->setIn_time_from(QDateTime());
    proxyModel->setIn_time_to(QDateTime());

    proxyModel->setExitNumber(0);
    proxyModel->setOut_time_from(QDateTime());
    proxyModel->setOut_time_to(QDateTime());

    proxyModel->invalidate();
    updateInfo();
}

void HistoryForm::updateInfo()
{
    int rows = proxyModel->rowCount();
    int tariff_rows = bDb.tariffModel->rowCount();
    int tariff_col = historyModel->fieldIndex("tariff_id");

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
    int childs_col = historyModel->fieldIndex("childs");
    for(int j=0; j<rows; j++){
        childs+=proxyModel->data(proxyModel->index(j,childs_col), Qt::EditRole).toInt();
    }
    if(childs)
        temp += QString("<span style=\" font-size:14pt; color:#55aaff;\">+%1 </span>"
                        "<span style=\" color:#646464;\">Детей без браслета</span></p>")
                .arg(childs);

    ui->info_label->setText(temp);
}
