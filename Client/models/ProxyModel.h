#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QDateTime>
#include <QSortFilterProxyModel>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ProxyModel(QObject *parent = 0);

    void setTariffId(const int &tariffId){tariff_id=tariffId;}
    void setBracerNumber(const int &num){bracer_number=num;}
    void setEnterNumber(const int &num){enter_number=num;}
    void setChildsNumber(const int &num){childs_number=num;}
    void setIn_time_from(const QDateTime &time){in_time_from=time;}
    void setIn_time_to(const QDateTime &time){in_time_to=time;}

    //history
    void setExitNumber(const int &num){exit_number=num;}
    void setOut_time_from(const QDateTime &time){out_time_from=time;}
    void setOut_time_to(const QDateTime &time){out_time_to=time;}
    //

    void invalidate();

    void setTimeOutEnabled(bool b){timeout=b;}
    void setWarningEnabled(bool b){warning=b;}

private:
    bool dateInRange(const QDateTime &dateTime, bool mode) const;

    QDateTime in_time_from;
    QDateTime in_time_to;

    int tariff_id;
    int bracer_number;
    int enter_number;
    int childs_number;

    //history
    int exit_number;
    QDateTime out_time_from;
    QDateTime out_time_to;
    //

    bool timeout;
    bool warning;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif // PROXYMODEL_H
