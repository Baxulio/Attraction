#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct ServerSettings {
        QString host;
        QString user;
        QString password;
        quint32 port;
    };
    struct BareerSettings{
        QString host;
        quint32 port;
    };

    struct SellingPointSettings{
        quint8 sellingPointNumber;
    };

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    ServerSettings serverSettings() const {return bServerSettings;}
    void setServerSettings(const ServerSettings server);

    BareerSettings bareerSettings() const {return bBareerSettings;}
    void setBareerSettings(const BareerSettings bareer);

    SellingPointSettings sellingPointSettings() const {return bSellingPointSettings;}
    void setSellingPointSettings(const SellingPointSettings point);

private:
    Ui::SettingsDialog *ui = nullptr;
    ServerSettings bServerSettings;
    BareerSettings bBareerSettings;
    SellingPointSettings bSellingPointSettings;

private slots:
    void apply();

private:
    void updateSettings();
};

#endif // SETTINGSDIALOG_H
