#ifndef DEALEXPORTDLG_H
#define DEALEXPORTDLG_H

#include <QDialog>
#include "dbutil.h"
#include <QCloseEvent>
#include <QList>
#include "struct.h"
#include <QMessageBox>
#include <QCheckBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QMap>
#include <QFileDialog>
#include "paramsettings.h"
#include <QTreeWidgetItemIterator>
#include <QProgressDialog>
#include <QTimer>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QCursor>
#include "dealexportpop.h"

namespace Ui {
class DealExportDlg;
}

class DealExportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DealExportDlg(QWidget *parent = 0);
    explicit DealExportDlg(DbUtil *db, QWidget *parent = 0);
    ~DealExportDlg();
    void closeEvent(QCloseEvent *e);

private:
    Ui::DealExportDlg *ui;
    DbUtil *db_util;
    int curExportType; //0成交，1持仓，2资金
    QList<StDept> deptList;
    QList<QString> infoIdList;
    QList<QMap<QString, QList<HoldInfo>>> holdInfoList;
    QList<QMap<QString, QList<DealInfo>>> dealInfoList;
    QList<QMap<QString, QList<FundInfo>>> fundInfoList;
    QString path;
    CParamSettings cset;
    bool isGenerate;
    QMap<QString, QStringList> params;
    int curParamRow;
    DealExportPop *dealPop;
    int curParamType; //0:按资金账号;1:按合约代码；2:按资金账号和合约代码;

    void generateDealInfo();
    void loadDealInfo();
    bool isTopItem(QTreeWidgetItem* item);
    void setChildCheckState(QTreeWidgetItem *item, Qt::CheckState cs);
    void setParentCheckState(QTreeWidgetItem *item);

public slots:
    void infoChanged();

private slots:
    void on_cm_type_activated(int index);
    void on_btn_reset_clicked();
    void on_btn_gn_clicked();
    void on_btn_export_clicked();
    void on_list_rela_itemChanged(QTreeWidgetItem *item, int column);
    void on_cus_menu(QAction *ac);
    void show_custom_context_menu(const QPoint &pos);
    void editParams(QMap<QString, QStringList> &map);
    void on_cm_param_currentIndexChanged(int index);
};


#endif // DEALEXPORTDLG_H
