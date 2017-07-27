#include "userinfomng.h"
#include "ui_userinfomng.h"

UserInfoMng::UserInfoMng(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoMng)
{
    ui->setupUi(this);
}

UserInfoMng::~UserInfoMng()
{
    delete ui;
}

UserInfoMng::UserInfoMng(DbUtil *db, UserInfo &curUser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoMng)
{
    ui->setupUi(this);

    model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("用户ID")));
    model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("用户名")));
    model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("所属部门")));
    model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("部门ID")));
    ui->tvIns->setModel(&model);
    ui->tvIns->show();
    model.setColumnCount(4);
    model.setRowCount(0);

    connect(ui->tvIns, &QTableView::customContextMenuRequested, this, &UserInfoMng::show_custom_context_menu);
    this->db_util = db;
    this->curUser.id = curUser.id;
    this->curUser.name = curUser.name;
    this->curUser.deptId = curUser.deptId;
    this->curUser.deptName = curUser.deptName;
    this->curUser.userType = curUser.userType;
    this->curUser.password = curUser.password;
    deptHaveChanged();
    loadInstanceTable("", "", "");;
}

void UserInfoMng::closeEvent(QCloseEvent *e)
{
    hide();
    QWidget *p = (QWidget *)parent();
    p->hide();
    e->ignore();
}

void UserInfoMng::setInstanceToRow(int row, UserInfo &ins)
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

void UserInfoMng::loadInstanceTable(QString id, QString name, QString deptId)
{
    db_util->getUserList(insList, id, name, deptId);
    model.setRowCount(insList.size());
    for (int i=0; i<insList.size(); i++) {
        UserInfo instance = insList.at(i);
        setInstanceToRow(i, instance);
    }
}

void UserInfoMng::show_custom_context_menu(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(this);
    QModelIndex cur = ui->tvIns->indexAt(pos);
    curRow = cur.row();
    curIns.id = model.item(curRow, 0)->text();
    if (curUser.userType == 0) {
        if (curIns.id == curUser.id) {
            popMenu->addAction(QStringLiteral("修改密码"));
        }
    } else if (curUser.userType == 1) {
        if (curIns.id == curUser.id) {
            popMenu->addAction(QStringLiteral("修改密码"));
        }
        popMenu->addAction(QStringLiteral("添加记录"));
        if (curRow >= 0) {
            popMenu->addAction(QStringLiteral("修改记录"));
            popMenu->addAction(QStringLiteral("删除记录"));
        }
    } else {}
    connect(popMenu, &QMenu::triggered, this, &UserInfoMng::on_cus_menu);
    popMenu->exec(QCursor::pos());
}

void UserInfoMng::on_cus_menu(QAction *ac)
{
    int row = curRow;
    if (row >=0) {
        curIns.id = model.item(row, 0)->text();
        curIns.name = model.item(row, 1)->text();
        curIns.deptName = model.item(row, 2)->text();
        curIns.deptId = model.item(row, 3)->text();
        QList<UserInfo> list;
        this->db_util->getUserList(list, curIns.id, curIns.name, curIns.deptId);
        if (list.size() == 1) {
            curIns.password = list.at(0).password;
            curIns.userType = list.at(0).userType;
        } else {
            curIns.password = cenc.encrypt("123456");
            curIns.userType = 0;
        }
    }
    if (ac->text() == QStringLiteral("修改密码")) {
        pwdPop = new UserPasswordPop();
        connect(pwdPop, &UserPasswordPop::passwordInfo, this, &UserInfoMng::updatePwd);
        pwdPop->show();
    }
    if (ac->text() == QStringLiteral("添加记录")) {
        UserInfo ins;
        ins.userType = 0;
        ins.password = cenc.encrypt("123456");
        pop = new UserInfoPop(0, ins, this->deptList);
        connect(pop, &UserInfoPop::instanceSaved, this, &UserInfoMng::opra_db);
        pop->show();
    }
    if (ac->text() == QStringLiteral("修改记录")) {
        int result = QMessageBox::information(this, QStringLiteral("修改警告"), QStringLiteral("确认要修改此条记录吗？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        pop = new UserInfoPop(1, curIns, this->deptList);
        connect(pop, &UserInfoPop::instanceSaved, this, &UserInfoMng::opra_db);
        pop->show();
    }
    if (ac->text() == QStringLiteral("删除记录")) {
        int result = QMessageBox::information(this, QStringLiteral("删除警告"), QStringLiteral("确认要删除此条记录吗？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        bool ok = db_util->deleteUser(curIns);
        if (ok) {
            model.removeRow(row, QModelIndex());
        }
    }
}

void UserInfoMng::updatePwd(QString old_pwd, QString new_pwd)
{
    if (cenc.encrypt(old_pwd) == curUser.password) {
        curUser.password = cenc.encrypt(new_pwd);
        this->db_util->updateUser(curUser, curUser);
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("修改密码成功"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("旧密码不匹配"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void UserInfoMng::opra_db(UserInfo &instance, int save_type)
{
    int row = curRow;
    if (row < 0) {
       bool ok = db_util->insertUser(instance);
       if (ok) {
           int a = model.rowCount();
           model.insertRow(a, QModelIndex());
           setInstanceToRow(a, instance);
       }
    } else {
        //添加
        if (save_type == 0) {
            bool ok = db_util->insertUser(instance);
            if (ok) {
                model.insertRow(row+1, QModelIndex());
                setInstanceToRow(row+1, instance);
            }
        }
        //编辑
        if (save_type == 1) {
            bool ok = db_util->updateUser(instance, curIns);
            if (ok) {
                setInstanceToRow(row, instance);
            }
        }
    }
}

void UserInfoMng::on_btnQuery_clicked()
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

void UserInfoMng::deptHaveChanged()
{
    ui->cm_dept->clear();
    ui->cm_dept->addItem(QStringLiteral("所有部门"));
    db_util->getDeptList(deptList, "", "");
    for (int i=0; i<deptList.size(); i++) {
        StDept d = deptList.at(i);
        ui->cm_dept->addItem(d.name);
    }
}

void UserInfoMng::updateInsDept(StDept dept, StDept old)
{
    this->db_util->updateUser(dept, old);
    on_btnQuery_clicked();
}
