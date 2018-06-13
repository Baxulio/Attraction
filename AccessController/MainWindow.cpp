#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    bDb(DatabaseManager::instance())
{
    ui->setupUi(this);

    connect(&bDb,&DatabaseManager::connectionChanged, [this](bool status){
        if(status){
            //emit showStatusMessage("<font color='green'>Successfully connected!");
        }
        else exit(EXIT_FAILURE);
    });

    bDb.connectToDatabase(bSettings->serverSettings().host,
                          bSettings->serverSettings().user,
                          bSettings->serverSettings().password,
                          bSettings->serverSettings().port);
}

MainWindow::~MainWindow()
{
    delete ui;
}
