#ifndef BROKERINFOPOP_H
#define BROKERINFOPOP_H

#include <QDialog>
#include "struct.h"
#include <QMessageBox>
#include <QList>

namespace Ui {
class BrokerInfoPop;
}

class BrokerInfoPop : public QDialog
{
    Q_OBJECT

public:
    explicit BrokerInfoPop(QWidget *parent = 0);
    explicit BrokerInfoPop(int save_type , BrokerInfo &ins, QList<StDept> &deptList, QWidget *parent = 0);
    ~BrokerInfoPop();

private:
    Ui::BrokerInfoPop *ui;
    //记录需要更新还是增加
    int ty;
    QList<StDept> deptList;

signals:
    void instanceSaved(BrokerInfo &ins, int save_type);

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // BROKERINFOPOP_H
