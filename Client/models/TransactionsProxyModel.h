#ifndef TRANSACTIONSPROXYMODEL_H
#define TRANSACTIONSPROXYMODEL_H

#include <QDateTime>
#include <QSortFilterProxyModel>

class TransactionsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    TransactionsProxyModel(QObject *parent = 0);

    void setTimeFrom(const QDateTime &time){time_from=time;}
    void setTimeTo(const QDateTime &time){time_to=time;}

    void setBracerNumebr(const int &num){bracer_number=num;}
    void setAmount(const int &num){amount=num;}
    void setActivityPoint(const int &num){activity_point=num;}
    void setProduct(const QString &str){product=str;}

    void setPriceFrom(const double &price){price_from=price;}
    void setPriceTo(const double &price){price_to=price;}

    void setTerminal(bool b){terminal=b;}
    void setMoney(bool b){money=b;}

    void setRegEnabled(bool b){reg=b;}
    void setBuyEnabled(bool b){buy=b;}
    void setPenaltyEnabled(bool b){penalty=b;}
    void setAddBallanceEnabled(bool b){addBallance=b;}
    void setReturnDebtEnabled(bool b){returnDebt=b;}
    void setReturnRemainderEnabled(bool b){returnRemainder=b;}

    void invalidate();

private:
    QDateTime time_from;
    QDateTime time_to;

    int bracer_number;
    int amount;
    int activity_point;
    QString product;

    double price_from;
    double price_to;

    bool terminal;
    bool money;

    bool reg;
    bool buy;
    bool penalty;
    bool addBallance;
    bool returnDebt;
    bool returnRemainder;

private:
    bool dateInRange(const QDateTime &dateTime) const;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // TRANSACTIONSPROXYMODEL_H
