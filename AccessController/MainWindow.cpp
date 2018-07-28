#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QSettings>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    bDb(DatabaseManager::instance()),
    server(new QTcpServer(this)),
    bSettings(new SettingsDialog(this)),
    fromClient(false)
{
    ui->setupUi(this);

    connect(&bTimer, &QTimer::timeout, this, &MainWindow::timeout);

    readSettings();
    initActionsConnections();

    if(!bSettings->modeSettings().mode and !server->listen(QHostAddress::AnyIPv4, 1234))
        close();
    connect(server, &QTcpServer::newConnection, [this]{
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, [this,socket]{

            fromClient=true;
            wiegandCallback(socket->readAll().toULongLong());
            //            if(arr==QByteArray::fromStdString("kuwoy")){
            //                fromClient=true;
            //                openBareer();
            //            }
            //            else if(arr==QByteArray::fromStdString("band")){
            //                fromClient=false;
            //                timeout();
            //            }
        });
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    });

    connect(&bDb,&DatabaseManager::connectionChanged, [this](const bool status){
        if(status){
            ui->statusBar->showMessage("Successfully connected!");
        }
        else close();
    });

    ui->connectButton->click();
}

MainWindow::~MainWindow()
{
    delete bSettings;
    delete ui;
}

void MainWindow::wiegandCallback(quint32 code)
{
    if(bSettings->modeSettings().mode){
        if(!enter(code))return;

        openBareer();
        return;
    }

    if(!exit(code))return;

    openBareer();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::initActionsConnections()
{
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

    if(!settings.contains("server_host"))
        return;

    SettingsDialog::ServerSettings server;
    server.host = settings.value("server_host",QString()).toString();
    server.user = settings.value("server_user",QString()).toString();
    server.password = settings.value("server_password",QString()).toString();
    server.port = settings.value("server_port",quint32()).toUInt();

    bSettings->setServerSettings(server);

    SettingsDialog::ModeSettings mode;
    mode.bareerNumber = settings.value("bareer_number", quint8()).toUInt();
    mode.mode = settings.value("mode",bool()).toBool();
    mode.wait_time = settings.value("wait_time", quint32()).toUInt();

    bSettings->setMode(mode);
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

    SettingsDialog::ModeSettings mode = bSettings->modeSettings();
    settings.setValue("bareer_number",mode.bareerNumber);
    settings.setValue("mode",mode.mode);
    settings.setValue("wait_time",mode.wait_time);
}

bool MainWindow::enter(quint32 code)
{
    QSqlQuery query;
    if(!query.exec(QString("SELECT id_code FROM staff WHERE id_code = %1;").arg(code))){
        bDb.debugQuery(query);
        return false;
    }
    query.next();
    if(query.isValid()){
        enterRec=query.record();

        if(!query.exec(QString("SELECT staff_id FROM staff_history WHERE staff_id = %1 AND ISNULL(exit_time) AND DATE(enter_time)=DATE(SYSDATE());").arg(code))){
            bDb.debugQuery(query);
            return false;
        }

        query.next();

        if(query.isValid())return false;

        return true;
    }

    if(!query.exec(QString("SELECT id, tariff_id FROM active_bracers WHERE code=%1 AND ISNULL(enter_time);").arg(code))){
        bDb.debugQuery(query);
        return false;
    }

    query.next();
    if(!query.isValid()){
        ui->statusBar->showMessage("Карта не зарегистрирована или не зафиксирована при входе!",2000);
        return false;
    }
    enterRec = query.record();
    return true;
}

bool MainWindow::exit(quint32 code)
{
    QSqlQuery query;

    if(fromClient){
        if(!query.exec(QString("SELECT id FROM active_bracers WHERE code=%1;").arg(code))){
            bDb.debugQuery(query);
            return false;
        }
        query.next();
        if(!query.isValid())
            return false;
        exitRec = query.record();
        return true;
    }
    if(!query.exec(QString("SELECT id FROM staff_history WHERE staff_id=%1 AND ISNULL(exit_time) AND DATE(enter_time)=DATE(SYSDATE());").arg(code))){
        bDb.debugQuery(query);
        return false;
    }
    query.next();
    if(!query.isValid())
        return false;

    exitRec=query.record();
    return true;
}

void MainWindow::interrupt()
{
    if(isInterrupted)return;

    isInterrupted=true;

    QSqlQuery query;

    switch(bSettings->modeSettings().mode){

    //ENTER
    case true: {
        if(enterRec.value("id_code").isValid()){

            if(!query.exec(QString("INSERT INTO staff_history(staff_id,enter_time) VALUES(%1,SYSDATE());")
                           .arg(enterRec.value("id_code").toUInt()))){
                bDb.debugQuery(query);
                return;
            }
            break;
        }

        if(!query.exec(QString("CALL enter('%1','%2', '%3');")
                       .arg(bSettings->modeSettings().bareerNumber)
                       .arg(enterRec.value("id").toInt())
                       .arg(enterRec.value("tariff_id").toInt()))){
            bDb.debugQuery(query);
            return;
        }
        break;
    }

        //EXIT
    case false: {
        if(fromClient){
            if(!query.exec(QString("CALL move_to_history('%1','%2');")
                           .arg(bSettings->modeSettings().bareerNumber)
                           .arg(exitRec.value("id").toInt()))){
                bDb.debugQuery(query);
                return;
            }
            break;
        }
        else if(!query.exec(QString("UPDATE staff_history SET exit_time=SYSDATE() WHERE id=%1;")
                            .arg(exitRec.value("id").toInt())))
        {
            bDb.debugQuery(query);
        }
    }
    }

    timeout();
}

void MainWindow::timeout()
{
    digitalWrite(BAREER_PIN,LOW);
    bTimer.stop();
    fromClient = false;
    enterRec.clearValues();
    exitRec.clearValues();
}

void MainWindow::openBareer()
{
    isInterrupted=false;
    digitalWrite(BAREER_PIN,HIGH);
    bTimer.start(bSettings->modeSettings().wait_time*1000);
}
