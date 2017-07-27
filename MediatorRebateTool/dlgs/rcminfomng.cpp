﻿#include "rcminfomng.h"
#include "ui_rcminfomng.h"

RcmInfoMng::RcmInfoMng(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RcmInfoMng)
{
    ui->setupUi(this);
}

RcmInfoMng::~RcmInfoMng()
{
    delete ui;
}

RcmInfoMng::RcmInfoMng(DbUtil *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RcmInfoMng)
{
    ui->setupUi(this);

    model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("开发人代码")));
    model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("开发人名称")));
    model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("所属部门")));
    model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("部门ID")));
    ui->tvIns->setModel(&model);
    ui->tvIns->show();
    model.setColumnCount(4);
    model.setRowCount(0);

    connect(ui->tvIns, &QTableView::customContextMenuRequested, this, &RcmInfoMng::show_custom_context_menu);
    this->db_util = db;
    deptHaveChanged();
    loadInstanceTable("", "", "");;
}

void RcmInfoMng::closeEvent(QCloseEvent *e)
{
    hide();
    QWidget *p = (QWidget *)parent();
    p->hide();
    e->ignore();
}

void RcmInfoMng::setInstanceToRow(int row, RcmInfo &ins)
{
    QModelIndex index;
    index = model.index(row, 0, QModelIndex());
    model.setData(index, QVariant(ins.id));
    index = model.index(row, 1, QModelIndex());
    model.setData(index, QVariant(ins.name));
    index = model.index(row, 2, QModelIndex());
    model.setData(index, QVariant(ins.deptName));
    index = model.index(row, 3, QModelIndex());
    model.setData(index, QVariant(ins.deptId));
}

void RcmInfoMng::loadInstanceTable(QString id, QString name, QString deptId)
{
    db_util->getRcmInfoList(insList, id, name, deptId);
    model.setRowCount(insList.size());
    for (int i=0; i<insList.size(); i++) {
        RcmInfo instance = insList.at(i);
        setInstanceToRow(i, instance);
    }
}

void RcmInfoMng::show_custom_context_menu(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(this);
    QModelIndex cur = ui->tvIns->indexAt(pos);
    curRow = cur.row();
    popMenu->addAction(QStringLiteral("添加记录"));
    if (curRow >= 0) {
        popMenu->addAction(QStringLiteral("选择记录"));
        popMenu->addAction(QStringLiteral("修改记录"));
        popMenu->addAction(QStringLiteral("删除记录"));
    }
    connect(popMenu, &QMenu::triggered, this, &RcmInfoMng::on_cus_menu);
    popMenu->exec(QCursor::pos());
}

void RcmInfoMng::on_cus_menu(QAction *ac)
{
    int row = curRow;
    if (row >=0) {
        curIns.id = model.item(row, 0)->text();
        curIns.name = model.item(row, 1)->text();
        curIns.deptName = model.item(row, 2)->text();
        curIns.deptId = model.item(row, 3)->text();
    }

    if (ac->text() == QStringLiteral("选择记录")) {
        emit sendInfoToRelaMng(curIns);
    }
    if (ac->text() == QStringLiteral("添加记录")) {
        RcmInfo ins;
        pop = new RcmInfoPop(0, ins, this->deptList);
        connect(pop, &RcmInfoPop::instanceSaved, this, &RcmInfoMng::opra_db);
        pop->show();
    }
    if (ac->text() == QStringLiteral("修改记录")) {
        int result = QMessageBox::information(this, QStringLiteral("修改警告"), QStringLiteral("修改该记录将会修改相关的客户关系,确认要修改？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        pop = new RcmInfoPop(1, curIns, this->deptList);
        connect(pop, &RcmInfoPop::instanceSaved, this, &RcmInfoMng::opra_db);
        pop->show();
    }
    if (ac->text() == QStringLiteral("删除记录")) {
        int result = QMessageBox::information(this, QStringLiteral("删除警告"), QStringLiteral("删除该记录将会删除相关的客户关系,确认要删除？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        bool ok = db_util->deleteRcmInfo(curIns);
        if (ok) {
            model.removeRow(row, QModelIndex());
            emit relaInfoDelete(curIns);
        }
    }
}

void RcmInfoMng::opra_db(RcmInfo &instance, int save_type)
{
    int row = curRow;
    if (row < 0) {
       bool ok = db_util->insertRcmInfo(instance);
       if (ok) {
           int a = model.rowCount();
           model.insertRow(a, QModelIndex());
           setInstanceToRow(a, instance);
       }
    } else {
        //添加
        if (save_type == 0) {
            bool ok = db_util->insertRcmInfo(instance);
            if (ok) {
                model.insertRow(row+1, QModelIndex());
                setInstanceToRow(row+1, instance);
            }
        }
        //编辑
        if (save_type == 1) {
            bool ok = db_util->updateRcmInfo(instance, curIns);
            if (ok) {
                setInstanceToRow(row, instance);
                emit relaInfoUpdate(instance, curIns);
            }
        }
    }
}

void RcmInfoMng::on_btnQuery_clicked()
{
    QString id = ui->le_id->text();
    QString name = ui->le_name->text();
    int index = ui->cm_dept->currentIndex();
    if (index > 0) {
        QString deptId = deptList.at(index-1).id;
        loadInstanceTable(id, name, deptId);
    } else {
        loadInstanceTable(id, name, "");
    }
}

void RcmInfoMng::deptHaveChanged()
{
    ui->cm_dept->clear();
    ui->cm_dept->addItem(QStringLiteral("所有部门"));
    db_util->getDeptList(deptList, "", "");
    for (int i=0; i<deptList.size(); i++) {
        StDept d = deptList.at(i);
        ui->cm_dept->addItem(d.name);
    }
}

void RcmInfoMng::updateInsDept(StDept dept, StDept old)
{
    this->db_util->updateRcmInfo(dept, old);
    on_btnQuery_clicked();
}