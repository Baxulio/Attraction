#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QSettings>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMenu>
#include <QMessageBox>

#include "dialogs/SettingsDialog.h"
#include "delegates/SlidingStackedWidget.h"

#include "forms/RegisterForm.h"
#include "forms/UniteForm.h"
#include "forms/StatusForm.h"
#include "forms/ProductsForm.h"
#include "forms/HistoryForm.h"
#include "forms/DashboardForm.h"
#include "forms/AdditionalSettingsForm.h"
#include "forms/SalesForm.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    bDb(DatabaseManager::instance()),
    bSettings(new SettingsDialog(this)),
    bStackedWidget(new SlidingStackedWidget(this))
{
    ui->setupUi(this);
    ui->stack_widget_layout->insertWidget(0,bStackedWidget,1);
    bStackedWidget->setSpeed(370);
    connect(bStackedWidget, &SlidingStackedWidget::currentChanged, [this](int n){
        if(qobject_cast<RegisterForm*>(bStackedWidget->widget(n)))ui->register_button->setChecked(true);
        else if(qobject_cast<UniteForm*>(bStackedWidget->widget(n)))ui->unite_button->setChecked(true);
        else if(qobject_cast<StatusForm*>(bStackedWidget->widget(n))){
            ui->status_button->setChecked(true);
            qobject_cast<StatusForm*>(bStackedWidget->widget(n))->on_retrieve_info_but_clicked();
        }
        else if(qobject_cast<ProductsForm*>(bStackedWidget->widget(n)))ui->products_button->setChecked(true);
        else if(qobject_cast<DashboardForm*>(bStackedWidget->widget(n)))ui->dashboard_but->setChecked(true);
        else if(qobject_cast<AdditionalSettingsForm*>(bStackedWidget->widget(n)))ui->additional_settings_button->setChecked(true);
        else if(qobject_cast<HistoryForm*>(bStackedWidget->widget(n)))ui->history_button->setChecked(true);
        else if(qobject_cast<SalesForm*>(bStackedWidget->widget(n)))ui->sales_button->setChecked(true);
    });

    connect(this, &MainWindow::showStatusMessage, [this](const QString &msg){
        this->ui->status_label->setText(msg);
    });

    connect(&timer, &QTimer::timeout, [this](){emit ui->autoUpdate_button->clicked();});

    readSettings();
    initActionsConnections();

    connect(&bDb,&DatabaseManager::connectionChanged, [this](bool status){
        if(status){
            emit showStatusMessage("<font color='green'>Successfully connected!");
            emit ui->autoUpdate_button->clicked();
        }
        else emit showStatusMessage(QString("<font color='red'>%1 </font><font color='grey'>Disconnected!").arg(bDb.lastError().databaseText()));

        ui->connectButton->setEnabled(!status);
        ui->disconnectButton->setEnabled(status);
        ui->splitter->setEnabled(status);
    });

    ui->connectButton->clicked();
}

MainWindow::~MainWindow()
{
    delete bSettings;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
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
        timer.start(1000);
    });
    connect(ui->action_autorefresh_5_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_5_sec->text());
        timer.start(5000);
    });
    connect(ui->action_autorefresh_10_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_10_sec->text());
        timer.start(10000);
    });
    connect(ui->action_autorefresh_20_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_20_sec->text());
        timer.start(20000);
    });
    connect(ui->action_autorefresh_35_sec, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_35_sec->text());
        timer.start(35000);
    });
    connect(ui->action_autorefresh_1_min, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_autorefresh_1_min->text());
        timer.start(60000);

    });
    connect(ui->action_disabled_autorefresh, &QAction::triggered, [this](){
        ui->autoUpdate_button->setText(ui->action_disabled_autorefresh->text());
        timer.stop();
    });
    ui->autoUpdate_button->setMenu(autoUpdate_menu);
    connect(ui->autoUpdate_button, &QToolButton::clicked, [this]{
        bDb.tariffModel->setTable("tariff");
        if(!bDb.tariffModel->select()){
            bDb.debugError(bDb.tariffModel->lastError());
            return;
        }
        ui->adult_tariff_price_label->setText(bDb.tariffModel->record(0).value("price").toString());
        ui->children_tariff_price_label->setText(bDb.tariffModel->record(1).value("price").toString());

        emit bDb.refresh();
    });
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
        for (int i = 0; i < bStackedWidget->count(); i++) {
            if(qobject_cast<RegisterForm*>(bStackedWidget->widget(i))){
                bStackedWidget->slideInIdx(i/*,SlidingStackedWidget::t_direction::AUTOMATIC*/);
                return;
            }
        }
        RegisterForm *registerForm = new RegisterForm(this);
        bStackedWidget->slideInIdx(bStackedWidget->addWidget(registerForm));
        connect(registerForm, &RegisterForm::back, [this, registerForm](){
            if(bStackedWidget->count()<2)return;
            bStackedWidget->removeWidget(registerForm);
            registerForm->deleteLater();
        });
    });
    connect(ui->unite_button, &QPushButton::toggled, [this](bool checked){
        if(!checked)return;
        for (int i = 0; i < bStackedWidget->count(); i++) {
            if(qobject_cast<UniteForm*>(bStackedWidget->widget(i))){
                bStackedWidget->slideInIdx(i/*,SlidingStackedWidget::t_direction::AUTOMATIC*/);
                return;
            }
        }
        UniteForm *uniteForm = new UniteForm(this);
        bStackedWidget->slideInIdx(bStackedWidget->addWidget(uniteForm));
        connect(uniteForm, &UniteForm::back, [this, uniteForm](){
            if(bStackedWidget->count()<2)return;
            bStackedWidget->removeWidget(uniteForm);
            uniteForm->deleteLater();
        });
    });
    connect(ui->status_button, &QPushButton::toggled, [this](bool checked){
        if(!checked)return;
        for (int i = 0; i < bStackedWidget->count(); i++) {
            if(qobject_cast<StatusForm*>(bStackedWidget->widget(i))){
                bStackedWidget->slideInIdx(i/*,SlidingStackedWidget::t_direction::AUTOMATIC*/);
                return;
            }
        }
        StatusForm *statusForm = new StatusForm(*bSettings, this);
        bStackedWidget->slideInIdx(bStackedWidget->addWidget(statusForm));
        connect(statusForm, &StatusForm::back, [this, statusForm](){
            if(bStackedWidget->count()<2)return;
            bStackedWidget->removeWidget(statusForm);
            statusForm->deleteLater();
        });
    });
    connect(ui->products_button, &QPushButton::toggled, [this](bool checked){
        if(!checked)return;
        for (int i = 0; i < bStackedWidget->count(); i++) {
            if(qobject_cast<ProductsForm*>(bStackedWidget->widget(i))){
                bStackedWidget->slideInIdx(i/*,SlidingStackedWidget::t_direction::AUTOMATIC*/);
                return;
            }
        }
        ProductsForm *productsForm = new ProductsForm(this);
        bStackedWidget->slideInIdx(bStackedWidget->addWidget(productsForm));
        connect(productsForm, &ProductsForm::back, [this, productsForm](){
            if(bStackedWidget->count()<2)return;
            bStackedWidget->removeWidget(productsForm);
            productsForm->deleteLater();
        });
    });
    connect(ui->dashboard_but, &QPushButton::toggled, [this](bool checked){
        if(!checked)return;
        for (int i = 0; i < bStackedWidget->count(); i++) {
            if(qobject_cast<DashboardForm*>(bStackedWidget->widget(i))){
                bStackedWidget->slideInIdx(i/*,SlidingStackedWidget::t_direction::AUTOMATIC*/);
                return;
            }
        }
        DashboardForm *dashboardForm = new DashboardForm(this);
        bStackedWidget->slideInIdx(bStackedWidget->addWidget(dashboardForm));
        connect(dashboardForm, &DashboardForm::back, [this, dashboardForm](){
            if(bStackedWidget->count()<2)return;
            bStackedWidget->removeWidget(dashboardForm);
            dashboardForm->deleteLater();
        });
    });
    connect(ui->additional_settings_button, &QPushButton::toggled, [this](bool checked){
        if(!checked)return;
        for (int i = 0; i < bStackedWidget->count(); i++) {
            if(qobject_cast<AdditionalSettingsForm*>(bStackedWidget->widget(i))){
                bStackedWidget->slideInIdx(i/*,SlidingStackedWidget::t_direction::AUTOMATIC*/);
                return;
            }
        }
        AdditionalSettingsForm *additionalSettingsForm = new AdditionalSettingsForm(this);
        bStackedWidget->slideInIdx(bStackedWidget->addWidget(additionalSettingsForm));
        connect(additionalSettingsForm, &AdditionalSettingsForm::back, [this, additionalSettingsForm](){
            if(bStackedWidget->count()<2)return;
            bStackedWidget->removeWidget(additionalSettingsForm);
            additionalSettingsForm->deleteLater();
        });
    });
    connect(ui->history_button, &QPushButton::toggled, [this](bool checked){
        if(!checked)return;
        for (int i = 0; i < bStackedWidget->count(); i++) {
            if(qobject_cast<HistoryForm*>(bStackedWidget->widget(i))){
                bStackedWidget->slideInIdx(i/*,SlidingStackedWidget::t_direction::AUTOMATIC*/);
                return;
            }
        }
        HistoryForm *historyForm = new HistoryForm(this);
        bStackedWidget->slideInIdx(bStackedWidget->addWidget(historyForm));
        connect(historyForm, &HistoryForm::back, [this, historyForm](){
            if(bStackedWidget->count()<2)return;
            bStackedWidget->removeWidget(historyForm);
            historyForm->deleteLater();
        });
    });
    connect(ui->sales_button, &QPushButton::toggled, [this](bool checked){
        if(!checked)return;
        for (int i = 0; i < bStackedWidget->count(); i++) {
            if(qobject_cast<SalesForm*>(bStackedWidget->widget(i))){
                bStackedWidget->slideInIdx(i/*,SlidingStackedWidget::t_direction::AUTOMATIC*/);
                return;
            }
        }
        SalesForm *salesForm = new SalesForm(this);
        bStackedWidget->slideInIdx(bStackedWidget->addWidget(salesForm));
        connect(salesForm, &SalesForm::back, [this, salesForm](){
            if(bStackedWidget->count()<2)return;
            bStackedWidget->removeWidget(salesForm);
            salesForm->deleteLater();
        });
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

    SettingsDialog::BareerSettings bareer;
    bareer.host = settings.value("bareer_host", QString()).toString();
    bareer.port = settings.value("bareer_port", quint32()).toUInt();

    bSettings->setBareerSettings(bareer);
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

    SettingsDialog::BareerSettings bareer = bSettings->bareerSettings();
    settings.setValue("bareer_host", bareer.host);
    settings.setValue("bareer_port", bareer.port);
}
