#ifndef DASHBOARDPROXYMODEL_H
#define DASHBOARDPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDateTime>

class DashboardProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DashboardProxyModel(QObject *parent = 0);

    void setBracerNumber(const int &num){bracer_number=num;}
    void setEnterNumber(const int &num){enter_number=num;}
    void setIn_time_from(const QDateTime &time){in_time_from=time;}
    void setIn_time_to(const QDateTime &time){in_time_to=time;}
    void setTariff_id(const int &id){tariff_id=id;}

    //history
    void setExitNumber(const int &num){exit_number=num;}
    void setOut_time_from(const QDateTime &time){out_time_from=time;}
    void setOut_time_to(const QDateTime &time){out_time_to=time;}
    //

    void invalidate();

private:
    bool dateInRange(const QDateTime &dateTime, bool mode) const;

    QDateTime in_time_from;
    QDateTime in_time_to;

    int bracer_number;
    int enter_number;
    int tariff_id;

    //history
    int exit_number;
    QDateTime out_time_from;
    QDateTime out_time_to;
    //

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

#endif // DASHBOARDPROXYMODEL_H
