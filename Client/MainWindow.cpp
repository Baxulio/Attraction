#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QSettings>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMenu>

#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

#include "SettingsDialog.h"
#include "RegisterForm.h"
#include "UniteForm.h"
#include "StatusForm.h"

#include "OperationsWithBracer.h"
#include "Monitoring.h"
#include "HistoryForm.h"
#include "PriceRules.h"

#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    bDb(DatabaseManager::instance()),
    bSettings(new SettingsDialog(this)),
    //    bOperationsWithBracer(new OperationsWithBracer(bSettings,this)),
    //    bMonitoring(new Monitoring(bSettings,this)),
    //    bHistoryForm(new HistoryForm(bSettings,this)),
    tariffModel(new QSqlTableModel(this))
{
    ui->setupUi(this);

    connect(this, &MainWindow::showStatusMessage, [this](const QString &msg){
        this->ui->status_label->setText(msg);
    });

    readSettings();
    initActionsConnections();

    connect(&bDb,&DatabaseManager::connectionChanged, [this](bool status){
        if(status){
            tariffModel->setTable("tariff");
            if(!tariffModel->select()){
                bDb.debugQuery(tariffModel->query());
                return;
            }
            emit showStatusMessage("<font color='green'>Successfully connected!");
        }
        else {
            emit showStatusMessage(QString("<font color='red'>%1 </font><font color='grey'>Disconnected!").arg(bDb.lastError().databaseText()));
        }
        ui->connectButton->setEnabled(!status);
        ui->disconnectButton->setEnabled(status);
        ui->splitter->setEnabled(status);
    });

    //    ui->tariff_comboBox->setModel(tariffModel);
    //    ui->tariff_comboBox->setModelColumn(1);
    //    ui->tariff_comboBox->setCurrentIndex(0);

    //    ui->status_tariff_comboBox->setModel(tariffModel);
    //    ui->status_tariff_comboBox->setModelColumn(1);

    //    statusModel->setTable("active_bracers");
    //    if(!statusModel->select()){
    //        showStatusMessage(QString("<font color='red'>%1").arg(statusModel->lastError().text()));
    //        makeDisconnection();
    //        return;
    //    }
    //    statusModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //    statusProxyModel->setSourceModel(statusModel);
    //    ui->status_tableView->setModel(statusProxyModel);
    //    statusProxyModel->setHeaders();

    //    makeConnection();
}

MainWindow::~MainWindow()
{
    delete bSettings;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //if (maybeSave()) {
    writeSettings();
    event->accept();
    //} else {
    //   event->ignore();
    //}
}

void MainWindow::initActionsConnections()
{
    /////////////////Autoupdate///////////////////////////////////////
    /// \brief autoUpdate_menu
    ///
    QMenu *autoUpdate_menu = new QMenu("Автообновление", this);
    autoUpdate_menu->addAction(ui->action_disabled_autorefresh);
    autoUpdate_menu->addAction(ui->action_autorefresh_1_sec);
    autoUpdate_menu->addAction(ui->action_autorefresh_5_sec);
    autoUpdate_menu->addAction(ui->action_autorefresh_10_sec);
    autoUpdate_menu->addAction(ui->action_autorefresh_20_sec);
    autoUpdate_menu->addAction(ui->action_autorefresh_35_sec);
    autoUpdate_menu->addAction(ui->action_autorefresh_1_min);
    connect(ui->action_autorefresh_1_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_1_sec->text());
        ui->autoUpdate_button->setAutoRepeat(true);
        ui->autoUpdate_button->setAutoRepeatDelay(1000);

    });
    connect(ui->action_autorefresh_5_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_5_sec->text());
        ui->autoUpdate_button->setAutoRepeat(true);
        ui->autoUpdate_button->setAutoRepeatDelay(5000);

    });
    connect(ui->action_autorefresh_10_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_10_sec->text());
        ui->autoUpdate_button->setAutoRepeat(true);
        ui->autoUpdate_button->setAutoRepeatDelay(10000);

    });
    connect(ui->action_autorefresh_20_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_20_sec->text());
        ui->autoUpdate_button->setAutoRepeat(true);
        ui->autoUpdate_button->setAutoRepeatDelay(20000);

    });
    connect(ui->action_autorefresh_35_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_35_sec->text());
        ui->autoUpdate_button->setAutoRepeat(true);
        ui->autoUpdate_button->setAutoRepeatDelay(35000);

    });
    connect(ui->action_autorefresh_1_min, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_1_min->text());
        ui->autoUpdate_button->setAutoRepeat(true);
        ui->autoUpdate_button->setAutoRepeatDelay(60000);

    });
    connect(ui->action_disabled_autorefresh, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_disabled_autorefresh->text());
        ui->autoUpdate_button->setAutoRepeat(false);
    });
    ui->autoUpdate_button->setMenu(autoUpdate_menu);
    connect(ui->autoUpdate_button, &QToolButton::clicked, [this](){emit bDb.refresh();});
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    connect(ui->configureButton, &QPushButton::clicked, bSettings, &SettingsDialog::show);
    connect(ui->aboutButton, &QPushButton::clicked, [this](){
        QMessageBox::about(this, tr("About Attraction"),
                           tr("The <b>Attraction</b> is an Access Controll System for amusement parks "
                              "with the following functionalities:"
                              "<ul><li>Maintain tariffs</li>"
                              "<li>Online deposit for visitors connected to their bracers</li>"
                              "<li>Selling any type of products</li>"
                              "<li>Work with Wiegand devices</li>"
                              "<li>Run on any platform</li>"
                              "<li>Controll the barrier<li>"
                              "<li>Print cheque with full transactions<li></ul>"
                              "<p align='right'>Author: <b>Nurnazarov Bakhmanyor Yunuszoda</b></p> <a align='right' href="
                              "'http://bahman.byethost18.com'>[bahman.byethost18.com]</a>"));
    });
    connect(ui->connectButton,&QPushButton::clicked, [this](){
        bDb.connectToDatabase(bSettings->serverSettings().host,
                              bSettings->serverSettings().user,
                              bSettings->serverSettings().password,
                              bSettings->serverSettings().port);
    });
    connect(ui->disconnectButton,&QPushButton::clicked, [this](){
        bDb.closeConnection();
    });
    connect(ui->register_button, &QPushButton::toggled, [this](bool checked){
       if(!checked)return;
       for (int i = 0; i < ui->stackedWidget->count(); i++) {
            if(dynamic_cast<RegisterForm*>(ui->stackedWidget->widget(i))){
                ui->stackedWidget->setCurrentIndex(i);
                return;
            }
       }
       RegisterForm *registerForm = new RegisterForm(this);
       ui->stackedWidget->setCurrentIndex(ui->stackedWidget->addWidget(registerForm));
    });
    connect(ui->unite_button, &QPushButton::toggled, [this](bool checked){
       if(!checked)return;
       for (int i = 0; i < ui->stackedWidget->count(); i++) {
            if(dynamic_cast<UniteForm*>(ui->stackedWidget->widget(i))){
                ui->stackedWidget->setCurrentIndex(i);
                return;
            }
       }
       UniteForm *uniteForm = new UniteForm(this);
       ui->stackedWidget->setCurrentIndex(ui->stackedWidget->addWidget(uniteForm));
    });
    connect(ui->status_button, &QPushButton::toggled, [this](bool checked){
       if(!checked)return;
       for (int i = 0; i < ui->stackedWidget->count(); i++) {
            if(dynamic_cast<StatusForm*>(ui->stackedWidget->widget(i))){
                ui->stackedWidget->setCurrentIndex(i);
                return;
            }
       }
       StatusForm *statusForm = new StatusForm(this);
       ui->stackedWidget->setCurrentIndex(ui->stackedWidget->addWidget(statusForm));
    });

}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    if(!settings.contains("server_host")){
        return;
    }
    SettingsDialog::ServerSettings server;
    server.host = settings.value("server_host",QString()).toString();
    server.user = settings.value("server_user",QString()).toString();
    server.password = settings.value("server_password",QString()).toString();
    server.port = settings.value("server_port",quint32()).toUInt();

    bSettings->setServerSettings(server);
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());

    SettingsDialog::ServerSettings server = bSettings->serverSettings();
    settings.setValue("server_host", server.host);
    settings.setValue("server_user", server.user);
    settings.setValue("server_password", server.password);
    settings.setValue("server_port", server.port);
}

//void MainWindow::on_check_bracer_status_pushButton_clicked()
//{
//    WieagandReaderDialog dialog(this);
//    if(dialog.exec() != QDialog::Accepted){
//        return;
//    }
//}

//void MainWindow::on_proceed_bracer_pushButton_clicked()
//{
//    WieagandReaderDialog dialog(this);
//    if(dialog.exec() != QDialog::Accepted){
//        return;
//    }
//    quint32 code=dialog.getCode();

//    QSqlQuery query;
//    if(!query.exec(QString("SELECT * FROM `Attraction`.`active_bracers` WHERE code=%1").arg(code))){
//        showStatusMessage(QString("<font color='red'>%1").arg(query.lastError().text()));
//        makeDisconnection();
//        return;
//    }
//    query.next();
//    if(query.isValid()){
//        QMessageBox::information(this, "Неожиданная ситуация",
//                                 QString("Браслет зарегистрирован!\n"
//                                         "Номер: %1\n"
//                                         "Время входа: %2\n"
//                                         "Номер входа: %3\n"
//                                         ).arg(query.value("bracer_number").toString())
//                                 .arg(query.value("enter_time").toDateTime().toString())
//                                 .arg(query.value("enter_number").toInt()));
//        return;
//    }
//    ui->tariff_comboBox->setModelColumn(0);
//    QSqlDatabase::database().transaction();
//    query.prepare("INSERT INTO `Attraction`.`deposit` (`cash`) VALUES (:cash);");
//    query.bindValue(":cash", ui->cash_doubleSpinBox->value());
//    query.exec();
//    int deposit_id = query.lastInsertId().toInt();

//    query.prepare("INSERT INTO `Attraction`.`active_bracers` (`code`, `bracer_number`, `deposit_id`, `tariff_id`) VALUES (:b_code, :b_bracer_number, :b_deposit_id, :b_tariff_id);");
//    query.bindValue(":b_code",code);
//    query.bindValue(":b_bracer_number",ui->bracer_number_spinBox->value());
//    query.bindValue(":b_deposit_id", deposit_id);
//    query.bindValue(":b_tariff_id",ui->tariff_comboBox->currentData(Qt::EditRole).toInt());
//    query.exec();

//    if(!QSqlDatabase::database().commit()){
//        showStatusMessage(QString("<font color='red'>%1").arg(QSqlDatabase::database().lastError().text()));
//        makeDisconnection();
//        return;
//    }
//    ui->tariff_comboBox->setModelColumn(1);
//}

//void MainWindow::on_start_unio_process_but_clicked()
//{
//    int countRow = ui->union_tableWidget->rowCount();
//    if(countRow<=1){
//        QMessageBox::information(this, "Неожиданная ситуация",
//                                 QString("Необходимы 2 или более браслета для их объединения!"));
//        return;
//    }
//    QSqlQuery query;
//    if(!query.exec(QString("SELECT deposit_id FROM `Attraction`.`active_bracers` WHERE code=%1")
//                   .arg(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(0,0)).toUInt()))){
//        showStatusMessage(QString("<font color='red'>%1").arg(query.lastError().text()));
//        makeDisconnection();
//        return;
//    }
//    query.next();
//    int depositId = query.value("deposit_id").toInt();
//    QString temp = QString("UPDATE `Attraction`.`active_bracers` SET `deposit_id`=%1 WHERE `code`= %2 ")
//            .arg(depositId)
//            .arg(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(1,0)).toUInt());

//    for(int i=2; i<countRow; i++){
//        temp+=QString("OR `code`=%1 ").arg(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(i,0)).toUInt());
//    }
//    if(!query.exec(temp+";")){
//        showStatusMessage(QString("<font color='red'>%1").arg(query.lastError().text()));
//        makeDisconnection();
//        return;
//    }
//    //UPDATE `Attraction`.`active_bracers` SET `deposit_id`=38 WHERE `code`=30828 OR `code`=30826;
//}

//void MainWindow::on_pushButton_clicked()
//{
//    WieagandReaderDialog dialog(this);
//    if(dialog.exec() != QDialog::Accepted){
//        return;
//    }
//    quint32 code=dialog.getCode();
//    QSqlQuery query;
//    if(!query.exec(QString("SELECT * FROM `Attraction`.`active_bracers` WHERE code=%1").arg(code))){
//        showStatusMessage(QString("<font color='red'>%1").arg(query.lastError().text()));
//        makeDisconnection();
//        return;
//    }
//    query.next();
//    if(!query.isValid()){
//        QMessageBox::information(this, "Неожиданная ситуация",
//                                 QString("Карта не зарегистрирована!"));
//        return;
//    }
//    int countRow = ui->union_tableWidget->rowCount();
//    for(int i=0; i<countRow; i++){
//        if(ui->union_tableWidget->model()->data(ui->union_tableWidget->model()->index(i,0)).toUInt()==code){
//            QMessageBox::information(this, "Неожиданная ситуация",
//                                     QString("Браслет уже добавлен в список!"));
//            return;
//        }

//    }
//    ui->union_tableWidget->insertRow(countRow);

//    ui->union_tableWidget->model()->setData(ui->union_tableWidget->model()->index(countRow,0),code);
//    ui->union_tableWidget->model()->setData(ui->union_tableWidget->model()->index(countRow,1),query.value("bracer_number").toInt());

//    if(!query.exec(QString("SELECT * FROM `Attraction`.`deposit` WHERE id=%1").arg(query.value("deposit_id").toInt()))){
//        showStatusMessage(QString("<font color='red'>%1").arg(query.lastError().text()));
//        makeDisconnection();
//        return;
//    }
//    query.next();
//    ui->union_tableWidget->model()->setData(ui->union_tableWidget->model()->index(countRow,2),query.value("cash").toDouble());
//    if(countRow==0){
//        ui->union_tableWidget->setVerticalHeaderLabels(QStringList()<<"Ведомый депозит: ");
//    }
//}

//void MainWindow::on_pushButton_2_clicked()
//{
//    ui->union_tableWidget->removeRow(ui->union_tableWidget->currentRow());
//}

//void MainWindow::on_salesMarket_triggered()
//{
//    if(ui->actionConnect->isEnabled())
//    {
//        showStatusMessage("Can't operate! First connect!");
//        return;
//    }
//    PriceRules priceRules(this);
//    priceRules.setModal(true);
//    priceRules.exec();
//}
