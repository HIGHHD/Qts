#ifndef DLGDEPTMNG_H
#define DLGDEPTMNG_H

#include <QDialog>
#include <QCloseEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDebug>
#include "dbutil.h"
#include "dlgdeptpop.h"
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QCursor>
#include <QMap>
#include <QList>
#include <QDebug>

namespace Ui {
class DlgDeptMng;
}

class DlgDeptMng : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDeptMng(QWidget *parent = 0);
    explicit DlgDeptMng(DbUtil *db_ut ,QWidget *parent = 0);
    ~DlgDeptMng();
    void closeEvent(QCloseEvent *e);

private:
    Ui::DlgDeptMng *ui;
    QStandardItemModel model;
    void loadDepts(QString id, QString name);
    void setDept(int row, StDept &dept);
    QList<StDept> deptList;
    DlgDeptPop *deptPop;
    DbUtil *db_util;
    int curRow;
    StDept curDept;

signals:
    //部门发生变动时改变combox中的列表
    void deptHaveChanged();
    //部门发生变动时改变其他对象的属性
    void shouldUpdateDept(StDept &dept, StDept &old);
public slots:

private slots:
    void on_cus_menu(QAction *ac);
    void show_dept_custom_context_menu(const QPoint &pos);
    void opra_dept_db(StDept &dept, int save_type);\
    void on_btnQuery_clicked();
};

#endif // DLGDEPTMNG_H
