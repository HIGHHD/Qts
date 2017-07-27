#ifndef RCMINFOPOP_H
#define RCMINFOPOP_H

#include <QDialog>
#include "struct.h"
#include <QMessageBox>
#include <QList>

namespace Ui {
class RcmInfoPop;
}

class RcmInfoPop : public QDialog
{
    Q_OBJECT

public:
    explicit RcmInfoPop(QWidget *parent = 0);
    explicit RcmInfoPop(int save_type , RcmInfo &ins, QList<StDept> &deptList, QWidget *parent = 0);
    ~RcmInfoPop();

private:
    Ui::RcmInfoPop *ui;
    //记录需要更新还是增加
    int ty;
    QList<StDept> deptList;

signals:
    void instanceSaved(RcmInfo &ins, int save_type);

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // RCMINFOPOP_H
