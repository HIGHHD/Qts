#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include "dbutil.h"
#include "dlglogin.h"
#include <QMdiArea>
#include <QSqlDatabase>
#include "struct.h"

#include "dlgs/clientinfomng.h"
#include "dlgs/agentinfomng.h"
#include "dlgs/brokerinfomng.h"
#include "dlgs/rcminfomng.h"
#include "dlgs/relationmng.h"
#include "dlgs/dlgdeptmng.h"
#include "dlgs/userinfomng.h"

#include "dlgs/dayimportdlg.h"
#include "dlgs/crmexportdlg.h"
#include "dlgs/dealexportdlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(DbUtil *db, QWidget *parent = 0);
    void initLogin(DlgLogin *dlg);
    DbUtil *db_util;
    UserInfo loginUser;
    ~MainWindow();
private:
    void initSubWin();
public slots:
    void quitSystem();
    void userManage();
    void deptManage();
    void mediatorManage();
    void clientManage();
    void agentManage();
    void rcmManage();
    void relationManage();

    void dayImportManage();
    void crmExportManage();
    void dealExportManage();

    void getLogin(UserInfo &user);
private:
    Ui::MainWindow *ui;
    DlgLogin *plogin;
    QMdiArea *mdiarea;
    DlgDeptMng *pdeptMng;

    UserInfoMng *userInfoMng;
    ClientInfoMng *clientInfoMng;
    AgentInfoMng *agentInfoMng;
    RcmInfoMng *rcmInfoMng;
    BrokerInfoMng *brokerInfoMng;
    RelationMng *relationMng;

    DayImportDlg *dayImportDlg;
    CrmExportDlg *crmExportDlg;
    QMdiSubWindow *subDayImport;
    DealExportDlg *dealExportDlg;
};

#endif // MAINWINDOW_H
