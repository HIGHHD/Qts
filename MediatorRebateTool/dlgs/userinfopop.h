#ifndef USERINFOPOP_H
#define USERINFOPOP_H

#include <QDialog>
#include <QMessageBox>
#include "struct.h"
#include <QList>

namespace Ui {
class UserInfoPop;
}

class UserInfoPop : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoPop(QWidget *parent = 0);
    explicit UserInfoPop(int save_type , UserInfo &ins, QList<StDept> &deptList, QWidget *parent = 0);
    ~UserInfoPop();

private:
    Ui::UserInfoPop *ui;
    //记录需要更新还是增加
    int ty;
    QList<StDept> deptList;
    UserInfo editUser;

signals:
    void instanceSaved(UserInfo &ins, int save_type);

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // USERINFOPOP_H
