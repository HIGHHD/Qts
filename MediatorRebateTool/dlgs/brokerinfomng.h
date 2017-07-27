#ifndef BROKERINFOMNG_H
#define BROKERINFOMNG_H

#include <QDialog>
#include <QTableView>
#include "dbutil.h"
#include "brokerinfopop.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QList>
#include "struct.h"
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QCursor>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class BrokerInfoMng;
}

class BrokerInfoMng : public QDialog
{
    Q_OBJECT

public:
    explicit BrokerInfoMng(QWidget *parent = 0);
    explicit BrokerInfoMng(DbUtil *db, QWidget *parent = 0);
    ~BrokerInfoMng();
    void closeEvent(QCloseEvent *e);

public slots:
    void  updateInsDept(StDept dept, StDept old);
    void deptHaveChanged();

private:
    Ui::BrokerInfoMng *ui;
    QStandardItemModel model;
    DbUtil *db_util;
    BrokerInfoPop *pop;
    QList<BrokerInfo> insList;
    int curRow;
    BrokerInfo curIns;
    QList<StDept> deptList;

    void setInstanceToRow(int row, BrokerInfo &ins);
    void loadInstanceTable(QString id, QString name, QString deptId);

public slots:

signals:
    void sendInfoToRelaMng(BrokerInfo &info);
    void relaInfoUpdate(BrokerInfo &info, BrokerInfo &old);
    void relaInfoDelete(BrokerInfo &info);

private slots:
    void on_btnQuery_clicked();
    void on_cus_menu(QAction *ac);
    void show_custom_context_menu(const QPoint &pos);
    void opra_db(BrokerInfo &instance, int save_type);

};

#endif // BROKERINFOMNG_H
