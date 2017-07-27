#include "dlgdeptmng.h"
#include "ui_dlgdeptmng.h"
#include <QContextMenuEvent>
#include <QModelIndexList>
#include <QMessageBox>

DlgDeptMng::DlgDeptMng(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDeptMng)
{
    ui->setupUi(this);
    //ui中已设置policy
    connect(ui->tvDept, &QTableView::customContextMenuRequested, this, &DlgDeptMng::show_dept_custom_context_menu);

    model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("部门代码")));
    model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("部门名称")));
    model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("所在城市")));
    model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("地址")));
    model.setHorizontalHeaderItem(4, new QStandardItem(QStringLiteral("电话")));
    model.setHorizontalHeaderItem(5, new QStandardItem(QStringLiteral("邮编")));
    model.setHorizontalHeaderItem(6, new QStandardItem(QStringLiteral("负责人")));

    ui->tvDept->setModel(&model);
    ui->tvDept->show();
    model.setColumnCount(7);
    model.setRowCount(0);
    loadDepts("", "");
}

DlgDeptMng::DlgDeptMng(DbUtil *db_ut ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDeptMng)
{
    ui->setupUi(this);
    //ui中已设置policy
    connect(ui->tvDept, &QTableView::customContextMenuRequested, this, &DlgDeptMng::show_dept_custom_context_menu);

    model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("部门代码")));
    model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("部门名称")));
    model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("所在城市")));
    model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("地址")));
    model.setHorizontalHeaderItem(4, new QStandardItem(QStringLiteral("电话")));
    model.setHorizontalHeaderItem(5, new QStandardItem(QStringLiteral("邮编")));
    model.setHorizontalHeaderItem(6, new QStandardItem(QStringLiteral("负责人")));

    this->db_util = db_ut;
    ui->tvDept->setModel(&model);
    ui->tvDept->show();
    model.setColumnCount(7);
    model.setRowCount(0);
    loadDepts("", "");
}

DlgDeptMng::~DlgDeptMng()
{
    delete ui;
}
void DlgDeptMng::closeEvent(QCloseEvent *e)
{
    hide();
    QWidget *p = (QWidget *)parent();
    p->hide();
    e->ignore();
}

void DlgDeptMng::setDept(int row, StDept &dept)
{
    QModelIndex index;
    index = model.index(row, 0, QModelIndex());
    model.setData(index, QVariant(dept.id));
    index = model.index(row, 1, QModelIndex());
    model.setData(index, QVariant(dept.name));
    index = model.index(row, 2, QModelIndex());
    model.setData(index, QVariant(dept.city));
    index = model.index(row, 3, QModelIndex());
    model.setData(index, QVariant(dept.address));
    index = model.index(row, 4, QModelIndex());
    model.setData(index, QVariant(dept.phone));
    index = model.index(row, 5, QModelIndex());
    model.setData(index, QVariant(dept.p_code));
    index = model.index(row, 6, QModelIndex());
    model.setData(index, QVariant(dept.leader));
}

void DlgDeptMng::loadDepts(QString id, QString name)
{
    db_util->getDeptList(deptList, id, name);
    model.setRowCount(deptList.size());
    for (int i=0; i<deptList.size(); i++) {
        StDept d = deptList.at(i);
        setDept(i, d);
    }
}

void DlgDeptMng::show_dept_custom_context_menu(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(this);
    QModelIndex cur = ui->tvDept->indexAt(pos);
    curRow = cur.row();
    popMenu->addAction(QStringLiteral("添加部门信息"));
    if (curRow >= 0) {
        popMenu->addAction(QStringLiteral("修改部门信息"));
        popMenu->addAction(QStringLiteral("删除部门信息"));
//        popMenu->addAction(QStringLiteral("批量删除"));
    }
    connect(popMenu, &QMenu::triggered, this, &DlgDeptMng::on_cus_menu);
    popMenu->exec(QCursor::pos());
}

void DlgDeptMng::on_cus_menu(QAction *ac)
{
    int row = curRow;
    if (row >=0) {
        curDept.id = model.item(row, 0)->text();
        curDept.name = model.item(row, 1)->text();
        curDept.city = model.item(row, 2)->text();
        curDept.address = model.item(row, 3)->text();
        curDept.phone = model.item(row, 4)->text();
        curDept.p_code = model.item(row, 5)->text();
        curDept.leader = model.item(row, 6)->text();
    }

    if (ac->text() == QStringLiteral("添加部门信息")) {
        StDept de;
        deptPop = new DlgDeptPop(0, de);
        connect(deptPop, &DlgDeptPop::deptSaved, this, &DlgDeptMng::opra_dept_db);
        deptPop->show();
    }
    if (ac->text() == QStringLiteral("修改部门信息")) {
        int result = QMessageBox::information(this, QStringLiteral("修改警告"), QStringLiteral("修改该记录将会修改相关部门的其他信息,确认要修改？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        deptPop = new DlgDeptPop(1, curDept);
        connect(deptPop, &DlgDeptPop::deptSaved, this, &DlgDeptMng::opra_dept_db);
        deptPop->show();
    }
    if (ac->text() == QStringLiteral("删除部门信息")) {
        int result = QMessageBox::information(this, QStringLiteral("删除"), QStringLiteral("确认要删除？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        bool ok = db_util->deleteDept(curDept);
        if (ok) {
            model.removeRow(row, QModelIndex());
            emit deptHaveChanged();
        }
    }
    if (ac->text() == QStringLiteral("批量删除")) {
        int result = QMessageBox::information(this, QStringLiteral("删除"), QStringLiteral("确认要删除？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        QModelIndexList selected = ui->tvDept->selectionModel()->selectedRows();
        QList<int> del_rows;
        foreach (QModelIndex index, selected) {
            int row = index.row();
            StDept dept;
            dept.id = model.item(row, 0)->text();
            dept.name = model.item(row, 1)->text();
            dept.city = model.item(row, 2)->text();
            dept.address = model.item(row, 3)->text();
            dept.phone = model.item(row, 4)->text();
            dept.p_code = model.item(row, 5)->text();
            dept.leader = model.item(row, 6)->text();
            bool ok = db_util->deleteDept(dept);
            if (ok) {
                del_rows.append(row);
            }
        }
        for (int i=0; i<del_rows.size(); i++) {
            model.removeRow(del_rows[i]);
            for (int j=0; j<del_rows.size(); j++) {
                del_rows[j] = del_rows[j] - 1;
            }
            qDebug() << del_rows;
        }
        emit deptHaveChanged();
    }
}

void DlgDeptMng::opra_dept_db(StDept &dept, int save_type)
{
    int row = curRow;
    if (row < 0) {
       bool ok = db_util->insertDept(dept);
       if (ok) {
           int a = model.rowCount();
           model.insertRow(a, QModelIndex());
           setDept(a, dept);
           emit deptHaveChanged();
       }
    } else {
        //添加
        if (save_type == 0) {
            bool ok = db_util->insertDept(dept);
            if (ok) {
                model.insertRow(row+1, QModelIndex());
                setDept(row+1, dept);
                emit deptHaveChanged();
            }
        }
        //编辑
        if (save_type == 1) {
            bool ok = db_util->updateDept(dept, curDept);
            if (ok) {
                setDept(row, dept);
                emit deptHaveChanged();
                emit shouldUpdateDept(dept, curDept);
            }
        }
    }
}


void DlgDeptMng::on_btnQuery_clicked()
{
    QString id_str = ui->leqryId->text();
    QString name_str = ui->leqryName->text();
    loadDepts(id_str, name_str);
}

