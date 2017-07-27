#include "userpasswordpop.h"
#include "ui_userpasswordpop.h"
#include <QMessageBox>

UserPasswordPop::UserPasswordPop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserPasswordPop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

UserPasswordPop::~UserPasswordPop()
{
    delete ui;
}

void UserPasswordPop::on_save_btn_clicked()
{
    int result = QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("确认修改密码？"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (result != QMessageBox::Yes) {
        return;
    }
    QString old_pwd = ui->le_old->text();
    QString new_pwd = ui->le_new->text();
    emit passwordInfo(old_pwd, new_pwd);
    close();
}

void UserPasswordPop::on_cancle_btn_clicked()
{
    close();
}
