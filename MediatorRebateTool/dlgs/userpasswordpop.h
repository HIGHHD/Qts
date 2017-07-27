#ifndef USERPASSWORDPOP_H
#define USERPASSWORDPOP_H

#include <QDialog>

namespace Ui {
class UserPasswordPop;
}

class UserPasswordPop : public QDialog
{
    Q_OBJECT

public:
    explicit UserPasswordPop(QWidget *parent = 0);
    ~UserPasswordPop();

private:
    Ui::UserPasswordPop *ui;

signals:
    void passwordInfo(QString old_pwd, QString new_pwd);

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // USERPASSWORDPOP_H
