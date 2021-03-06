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

    struct ActivityPointSettings{
        quint8 activityPointNumber;
    };

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    ServerSettings serverSettings() const {return bServerSettings;}
    void setServerSettings(const ServerSettings server);

    BareerSettings bareerSettings() const {return bBareerSettings;}
    void setBareerSettings(const BareerSettings bareer);

    ActivityPointSettings activityPointSettings() const {return bActivityPointSettings;}
    void setActivityPointSettings(const ActivityPointSettings point);

private:
    Ui::SettingsDialog *ui = nullptr;
    ServerSettings bServerSettings;
    BareerSettings bBareerSettings;
    ActivityPointSettings bActivityPointSettings;

private slots:
    void apply();

private:
    void updateSettings();
};

#endif // SETTINGSDIALOG_H
