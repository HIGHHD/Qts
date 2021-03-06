﻿#include "agentinfopop.h"
#include "ui_agentinfopop.h"

AgentInfoPop::AgentInfoPop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentInfoPop)
{
    ui->setupUi(this);
}

AgentInfoPop::~AgentInfoPop()
{
    delete ui;
}

AgentInfoPop::AgentInfoPop(int save_type, AgentInfo &ins, QList<StDept> &deptList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentInfoPop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    this->ty = save_type;

    ui->cm_dept->clear();
    ui->cm_dept->addItem(QStringLiteral("未选择"));
    int curIndex = 0;
    for (int i=0; i<deptList.size(); i++) {
        StDept dept = deptList.at(i);
        this->deptList.append(dept);
        ui->cm_dept->addItem(dept.name);
        if (ins.deptId == dept.id) {
            curIndex = i + 1;
        }
    }

    if (!ins.id.isEmpty()) {
        ui->le_id->setText(ins.id);
        ui->le_name->setText(ins.name);
        ui->cm_dept->setCurrentIndex(curIndex);
    }
}

void AgentInfoPop::on_save_btn_clicked()
{
    AgentInfo instance;
    instance.id = ui->le_id->text();
    instance.name = ui->le_name->text();
    int index = ui->cm_dept->currentIndex();
    if (index > 0) {
        StDept dept = this->deptList.at(index-1);
        instance.deptId = dept.id;
        instance.deptName = dept.name;
    } else {
        instance.deptName = ui->cm_dept->currentText();
    }

    if (instance.id.isEmpty() || instance.name.isEmpty()) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("代理人代码和名称不能为空"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        emit instanceSaved(instance, ty);
        close();
    }
}

void AgentInfoPop::on_cancle_btn_clicked()
{
    close();
}
