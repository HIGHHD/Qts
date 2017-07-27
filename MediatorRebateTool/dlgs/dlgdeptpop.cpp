#include "dlgdeptpop.h"
#include "ui_dlgdeptpop.h"
#include <QMessageBox>

DlgDeptPop::DlgDeptPop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDeptPop)
{
    ui->setupUi(this);
}

DlgDeptPop::DlgDeptPop(int save_type, StDept &dept, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDeptPop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ty = save_type;
    if (!dept.id.isEmpty()) {
        ui->l_id->setText(dept.id);
        ui->l_name->setText(dept.name);
        ui->l_city->setText(dept.city);
        ui->l_addr->setText(dept.address);
        ui->l_phone->setText(dept.phone);
        ui->l_pcode->setText(dept.p_code);
        ui->l_leader->setText(dept.leader);
    }
}

DlgDeptPop::~DlgDeptPop()
{
    delete ui;
}

void DlgDeptPop::on_save_btn_clicked()
{
    StDept dept;
    dept.id = ui->l_id->text();
    dept.name = ui->l_name->text();
    dept.city = ui->l_city->text();
    dept.address = ui->l_addr->text();
    dept.phone = ui->l_phone->text();
    dept.p_code = ui->l_pcode->text();
    dept.leader = ui->l_leader->text();
    if (dept.id.isEmpty() || dept.name.isEmpty()) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("部门代码和名称不能为空"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        emit deptSaved(dept, ty);
        close();
    }
}

void DlgDeptPop::on_cancle_btn_clicked()
{
    close();
}
