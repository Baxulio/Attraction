<<<<<<< HEAD
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <DatabaseManager.h>

#include "dialogs/SettingsDialog.h"
#include "Core.h"
#include "wiringPi.h"

#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void wiegandCallback(quint32 code);

private:
    Ui::MainWindow *ui;
    DatabaseManager &bDb;

    SettingsDialog *bSettings;
    QTimer bTimer;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initActionsConnections();
    void readSettings();
    void writeSettings();

    bool enter(quint32 code);
    bool exit(quint32 core);

public slots:
    void interrupt();
};

#endif // MAINWINDOW_H
=======
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DatabaseManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DatabaseManager &bDb;


};

#endif // MAINWINDOW_H
>>>>>>> 2dc2b20d8b348c141005f84cc59edd37cddbbdd6
