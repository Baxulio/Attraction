#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <DatabaseManager.h>

namespace Ui {
class MainWindow;
}

class SettingsDialog;
class QLabel;
class QPushButton;
class ProxyModel;
class QSqlTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *label;
    QPushButton *refreshButton;
    DatabaseManager &bDb;
    SettingsDialog *bSettings;

    QSqlTableModel *statusModel, *historyModel, *tariffModel;
    ProxyModel *statusProxyModel, *historyProxyModel;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void about();
    void showStatusMessage(const QString &message);
    void makeConnection();
    void makeDisconnection();

    void on_check_bracer_status_pushButton_clicked();

    void on_proceed_bracer_pushButton_clicked();

    void on_start_unio_process_but_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    void initActionsConnections();

    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
