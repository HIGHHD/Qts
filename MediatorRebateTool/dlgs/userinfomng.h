#ifndef USERINFOMNG_H
#define USERINFOMNG_H

#include <QDialog>
#include <QTableView>
#include "dbutil.h"
#include "userinfopop.h"
#include "userpasswordpop.h"
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
#include "fileencrypt.h"

namespace Ui {
class UserInfoMng;
}

class UserInfoMng : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoMng(QWidget *parent = 0);
    explicit UserInfoMng(DbUtil *db, UserInfo &curUser, QWidget *parent = 0);
    ~UserInfoMng();
    void closeEvent(QCloseEvent *e);

private:
    Ui::UserInfoMng *ui;
    QStandardItemModel model;
    DbUtil *db_util;
    UserPasswordPop *pwdPop;
    UserInfoPop *pop;
    QList<UserInfo> insList;
    UserInfo curUser;
    int curRow;
    UserInfo curIns;
    QList<StDept> deptList;
    CEncrypt cenc;

    void setInstanceToRow(int row, UserInfo &ins);
    void loadInstanceTable(QString id, QString name, QString deptId);

public slots:
    void updateInsDept(StDept dept, StDept old);
    void deptHaveChanged();

signals:
    void sendInfoToRelaMng(RcmInfo &info);
    void relaInfoUpdate(RcmInfo &info, RcmInfo &old);
    void relaInfoDelete(RcmInfo &info);

private slots:
    void on_btnQuery_clicked();
    void on_cus_menu(QAction *ac);
    void show_custom_context_menu(const QPoint &pos);
    void opra_db(UserInfo &instance, int save_type);
    void updatePwd(QString old_pwd, QString new_pwd);
};

#endif // USERINFOMNG_H
