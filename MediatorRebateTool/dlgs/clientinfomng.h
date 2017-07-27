#ifndef CLIENTINFOMNG_H
#define CLIENTINFOMNG_H

#include <QDialog>
#include <QTableView>
#include "dbutil.h"
#include "clientinfopop.h"
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
#include <QRegExp>
#include <QRegExpValidator>

namespace Ui {
class ClientInfoMng;
}

class ClientInfoMng : public QDialog
{
    Q_OBJECT

public:
    explicit ClientInfoMng(QWidget *parent = 0);
    explicit ClientInfoMng(DbUtil *db, QWidget *parent = 0);
    void closeEvent(QCloseEvent *e);
    ~ClientInfoMng();

    QList<StDept> deptList;

private:
    Ui::ClientInfoMng *ui;
    QStandardItemModel model;
    DbUtil *db_util;
    int pageNum;
    int curPageNum;
    ClientInfoPop *pop;
    QList<ClientInfo> insList;
    int curRow;
    ClientInfo curIns;

    void setInstanceToRow(int row, ClientInfo &ins);
    void loadInstanceTable(QString id, QString name, QDate start, QDate end, int page);

public slots:

signals:
    void sendInfoToRelaMng(ClientInfo &info);
    void relaInfoUpdate(ClientInfo &info, ClientInfo &old);
    void relaInfoDelete(ClientInfo &info);

private slots:
    void on_btnQuery_clicked();
    void on_btnGoFirst_clicked();\
    void on_btnGoPre_clicked();
    void on_btnGoNext_clicked();
    void on_btnGoLast_clicked();
    void on_btnGoNum_clicked();
    void on_cus_menu(QAction *ac);
    void show_custom_context_menu(const QPoint &pos);
    void opra_db(ClientInfo &instance, int save_type);
    void on_btnClear_clicked();
};

#endif // CLIENTINFOMNG_H
