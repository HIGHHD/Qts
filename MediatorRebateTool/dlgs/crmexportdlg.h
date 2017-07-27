#ifndef CRMEXPORTDLG_H
#define CRMEXPORTDLG_H

#include <QDialog>
#include "dbutil.h"
#include <QCloseEvent>
#include <QList>
#include "struct.h"
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QMap>
#include <QFileDialog>
#include "paramsettings.h"
#include <QTreeWidgetItemIterator>
#include <QProgressDialog>
#include <QTimer>

namespace Ui {
class CrmExportDlg;
}

class CrmExportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CrmExportDlg(QWidget *parent = 0);
    explicit CrmExportDlg(DbUtil *db, QWidget *parent = 0);
    ~CrmExportDlg();
    void closeEvent(QCloseEvent *e);

private slots:
    void on_cm_rela_activated(int index);
    void on_btn_reset_clicked();
    void on_btn_gn_clicked();
    void on_btn_export_clicked();
    void on_list_rela_itemChanged(QTreeWidgetItem *item, int column);

private:
    Ui::CrmExportDlg *ui;
    DbUtil *db_util;
    int curRelaType; //0居间，1代理，2开发
    QList<StDept> deptList;
    QList<QString> infoIdList;
    QList<BrokerCrm> brokerCrmList;
    QList<AgentCrm> agentCrmList;
    QList<RcmCrm> rcmCrmList;
    QString path;
    CParamSettings cset;
    bool isGenerate;

    void generateCrmInfo();
    void loadCrmInfo();
    bool isTopItem(QTreeWidgetItem* item);
    void setChildCheckState(QTreeWidgetItem *item, Qt::CheckState cs);
    void setParentCheckState(QTreeWidgetItem *item);

};

#endif // CRMEXPORTDLG_H
