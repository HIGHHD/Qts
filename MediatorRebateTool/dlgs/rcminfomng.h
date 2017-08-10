#ifndef RCMINFOMNG_H
#define RCMINFOMNG_H

#include <QDialog>
#include <QTableView>
#include "dbutil.h"
#include "rcminfopop.h"
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
class RcmInfoMng;
}

class RcmInfoMng : public QDialog
{
    Q_OBJECT

public:
    explicit RcmInfoMng(QWidget *parent = 0);
    explicit RcmInfoMng(DbUtil *db, QWidget *parent = 0);
    ~RcmInfoMng();
    void closeEvent(QCloseEvent *e);

public slots:
    void updateInsDept(StDept dept, StDept old);
    void deptHaveChanged();

private:
    Ui::RcmInfoMng *ui;
    QStandardItemModel model;
    DbUtil *db_util;
    RcmInfoPop *pop;
    QList<RcmInfo> insList;
    int curRow;
    RcmInfo curIns;
    QList<StDept> deptList;

    void setInstanceToRow(int row, RcmInfo &ins);
    void loadInstanceTable(QString id, QString name, QString deptId);

signals:
    void sendInfoToRelaMng(RcmInfo &info);
    void relaInfoUpdate(RcmInfo &info, RcmInfo &old);
    void relaInfoDelete(RcmInfo &info);

private slots:
    void on_btnQuery_clicked();
    void on_cus_menu(QAction *ac);
    void show_custom_context_menu(const QPoint &pos);
    void opra_db(RcmInfo &instance, int save_type);
    void on_tvIns_doubleClicked(const QModelIndex &index);
};

#endif // RCMINFOMNG_H
