#ifndef DLGDEPTPOP_H
#define DLGDEPTPOP_H

#include <QDialog>
#include "struct.h"

namespace Ui {
class DlgDeptPop;
}

class DlgDeptPop : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDeptPop(QWidget *parent = 0);
    explicit DlgDeptPop(int save_type, StDept &dept, QWidget *parent = 0);
    ~DlgDeptPop();
    //记录需要更新还是增加
    int ty;
    StDept dept;

private slots:
    void on_save_btn_clicked();

    void on_cancle_btn_clicked();

signals:
    void deptSaved(StDept &dept, int save_type);

private:
    Ui::DlgDeptPop *ui;
};

#endif // DLGDEPTPOP_H
