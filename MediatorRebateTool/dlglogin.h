#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "dbutil.h"
#include "fileencrypt.h"
#include "dlgserver.h"
#include <QListWidget>
#include "accountitem.h"
#include <QList>
#include "struct.h"
#include "paramsettings.h"
#include <QSqlDatabase>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QCursor>
#include <QTableWidgetItem>


namespace Ui {
class DlgLogin;
}

class DlgLogin : public QDialog
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    explicit DlgLogin(QWidget *parent = 0);
    explicit DlgLogin(DbUtil *db_util ,QWidget *parent = 0);
    DlgServer *dlg_sv;
    DbUtil *db_util;
    ~DlgLogin();
signals:
    void doLogin(UserInfo &user);
    //提供当前的stdb以及当前的dblist
    void editDbInfo(StDb &db_info, bool& isSuccess);
private:
    Ui::DlgLogin *ui;
    QListWidget *plwUser;
    DlgServer *pserver;
    int smallH;
    int bigH;
    int width;
    bool showServer;
    QList<StUser> userList;
    UserInfo loginUser;
    QList<StDb> dbList;
    int curDb;
    int curUser;
    CParamSettings set;
    QTableWidgetItem *curRightSel;
    CEncrypt cenc;

    void initUserList();
public slots:
    void showAccount(QString account);
    void removeAccount(QString account);
    void on_btnLogin_clicked();
    void on_btnQuit_clicked();
    void on_btnServer_clicked();
    void on_btnSelect_clicked();
    void on_btnCancel_clicked();
    void on_btnAdd_clicked();
    void shouldReloadSvTable(QList<StDb>& dbList, int curIndex);
    void checkUser();
private slots:
    void show_sv_custom_context_menu(const QPoint &pos);
    void addServer();
    void selectServer();
    void editServer();
    void deleteServer();
};

#endif //DLGLOGIN_H
