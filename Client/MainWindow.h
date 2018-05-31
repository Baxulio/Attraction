#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <DatabaseManager.h>

namespace Ui {
class MainWindow;
}

class SettingsDialog;
class OperationsWithBracer;
class Monitoring;
class HistoryForm;

class QSqlTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DatabaseManager &bDb;

    SettingsDialog *bSettings;
    OperationsWithBracer *bOperationsWithBracer;
    Monitoring *bMonitoring;
    HistoryForm *bHistoryForm;

    QSqlTableModel *tariffModel;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
//    void on_check_bracer_status_pushButton_clicked();

//    void on_proceed_bracer_pushButton_clicked();

//    void on_start_unio_process_but_clicked();

//    void on_pushButton_clicked();

//    void on_pushButton_2_clicked();

//    void on_salesMarket_triggered();

private:
    void initActionsConnections();
    void readSettings();
    void writeSettings();

signals:
    void showStatusMessage(const QString &message);
};

#endif // MAINWINDOW_H
