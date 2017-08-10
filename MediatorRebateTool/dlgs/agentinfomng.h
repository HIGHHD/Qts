#ifndef AGENTINFOMNG_H
#define AGENTINFOMNG_H

#include <QDialog>
#include <QTableView>
#include "dbutil.h"
#include "agentinfopop.h"
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
class AgentInfoMng;
}

class AgentInfoMng : public QDialog
{
    Q_OBJECT

public:
    explicit AgentInfoMng(QWidget *parent = 0);
    explicit AgentInfoMng(DbUtil *db, QWidget *parent = 0);
    ~AgentInfoMng();
    void closeEvent(QCloseEvent *e);

public slots:
    void updateInsDept(StDept dept, StDept old);
    void deptHaveChanged();

private:
    Ui::AgentInfoMng *ui;
    QStandardItemModel model;
    DbUtil *db_util;
    AgentInfoPop *pop;
    QList<AgentInfo> insList;
    int curRow;
    AgentInfo curIns;
    QList<StDept> deptList;

    void setInstanceToRow(int row, AgentInfo &ins);
    void loadInstanceTable(QString id, QString name, QString deptId);

signals:
    void sendInfoToRelaMng(AgentInfo &info);
    void relaInfoUpdate(AgentInfo &info, AgentInfo &old);
    void relaInfoDelete(AgentInfo &info);

private slots:
    void on_btnQuery_clicked();
    void on_cus_menu(QAction *ac);
    void show_custom_context_menu(const QPoint &pos);
    void opra_db(AgentInfo &instance, int save_type);

    void on_tvIns_doubleClicked(const QModelIndex &index);
};

#endif // AGENTINFOMNG_H
