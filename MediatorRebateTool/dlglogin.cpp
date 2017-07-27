#include "dlglogin.h"
#include "ui_dlglogin.h"
#include <QProxyStyle>
#include <QContextMenuEvent>

class LineEditStyle : public QProxyStyle
{
public:
    LineEditStyle(QStyle *style = 0) : QProxyStyle(style) { }

    int styleHint(StyleHint hint, const QStyleOption * option = 0,
                  const QWidget * widget = 0, QStyleHintReturn * returnData = 0 ) const
    {
        if (hint==QStyle::SH_LineEdit_PasswordCharacter)
            return '$';
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};

DlgLogin::DlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLogin)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

DlgLogin::DlgLogin(DbUtil *db_util ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLogin)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->lePwd->setStyle(new LineEditStyle(ui->lePwd->style()));
    bigH = this->geometry().height();
    smallH = bigH - 300;
    width = this->geometry().width();
    showServer = true;
    setFixedSize(width, smallH);

    connect(ui->cbId, &MyComBox::checkUser, this, &DlgLogin::checkUser);
    initUserList();

    //数据库连接相关设置
    this->db_util = db_util;
    connect(this, &DlgLogin::editDbInfo, this->db_util, &DbUtil::initConn);

    connect(ui->tvServer, &QTableWidget::customContextMenuRequested, this, &DlgLogin::show_sv_custom_context_menu);
    ui->tvServer->horizontalHeader()->setSectionsClickable(false);
    set.getDbList(dbList);
    curDb = set.getDbIndex();
    shouldReloadSvTable(dbList, curDb);
//    CEncrypt encrypt;
//    QString tmp = encrypt.encrypt(QStringLiteral("123456"));
//    qDebug() << tmp;
//    qDebug() << encrypt.uncrypt(tmp);
}

DlgLogin::~DlgLogin()
{
    delete ui;
}

void DlgLogin::closeEvent(QCloseEvent *e)
{
    e->accept();
    //qDebug() << "quit application!" ;
}

void DlgLogin::initUserList()
{
    plwUser = new QListWidget(ui->cbId);
    ui->cbId->setModel(plwUser->model());
    ui->cbId->setView(plwUser);
    ui->cbId->setEditable(true);
    ui->cbId->setCurrentIndex(-1);

    set.getUserList(userList);
    curUser = set.getUserIndex();
    //read config file to get account list
    for(int i=0; i<userList.size(); i++) {
        AccountItem *account_item = new AccountItem(ui->cbId);
        StUser user = userList[i];
        account_item->setAccountNumber(user.userId);
        connect(account_item, SIGNAL(showAccount(QString)), this, SLOT(showAccount(QString)));
        connect(account_item, SIGNAL(removeAccount(QString)), this, SLOT(removeAccount(QString)));
        QListWidgetItem *list_item = new QListWidgetItem(plwUser);
        plwUser->setItemWidget(list_item, account_item);
    }
    if (curUser > 0) {
        StUser user = userList[curUser-1];
        ui->cbId->setCurrentText(user.userId);
//        ui->cbId->setEditText(user.userId);
//        ui->cbId->setCurrentIndex(curUser);
        if (user.saveFlag == 1) {
            ui->cbSavePwd->setChecked(true);
            ui->lePwd->setText(cenc.uncrypt(user.password));
        }
        ui->lbUser->setText(user.username);
    }
}

void DlgLogin::checkUser()
{
    bool existFlag = false;
    for (int i=0; i<userList.size(); i++) {
        StUser user = userList[i];
        QString userId = ui->cbId->currentText();
        if (user.userId == userId) {
            existFlag = true;
            ui->lbUser->setText(user.username);
            if (user.saveFlag == 1) {
                ui->cbSavePwd->setChecked(true);
                ui->lePwd->setText(cenc.uncrypt(user.password));
            } else {
                ui->cbSavePwd->setChecked(false);
                ui->lePwd->setText("");
            }
            break;
        }
    }
    if (!existFlag) {
        ui->lbUser->setText("");
        ui->cbSavePwd->setChecked(false);
        ui->lePwd->setText("");
    }
}

void DlgLogin::showAccount(QString account)
{
    int list_count = plwUser->count();
    for (int i=0; i<list_count; i++) {
        QListWidgetItem *item = plwUser->item(i);
        AccountItem *account_item = (AccountItem *)(plwUser->itemWidget(item));
        QString account_number = account_item->getAccountNumber();
        if(account == account_number) {
            curUser = i + 1;
            break;
        }
    }
    if (curUser > 0) {
        set.setUserIndex(curUser);
        StUser user = userList[curUser-1];
        ui->cbId->setCurrentText(user.userId);
        if (user.saveFlag == 1) {
            ui->cbSavePwd->setChecked(true);
            ui->lePwd->setText(cenc.uncrypt(user.password));
        } else {
            ui->cbSavePwd->setChecked(false);
            ui->lePwd->setText("");
        }
        ui->lbUser->setText(user.username);
    }
    ui->cbId->hidePopup();
}
void DlgLogin::removeAccount(QString account)
{
    int result = QMessageBox::information(this, QStringLiteral("删除"), QStringLiteral("确认要删除账号？"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (result != QMessageBox::Yes) {
        return;
    }
    int list_count = plwUser->count();
    int j = 0;
    for (int i=0; i<list_count; i++) {
        QListWidgetItem *item = plwUser->item(i);
        AccountItem *account_item = (AccountItem *)(plwUser->itemWidget(item));
        QString account_number = account_item->getAccountNumber();
        if(account == account_number) {
            j = i;
            plwUser->takeItem(i);
            delete item;
            break;
        }
    }
    userList.removeAt(j);
    set.setUserList(userList);
    set.setUserIndex(0);
    ui->cbSavePwd->setChecked(false);
    ui->lePwd->setText("");
    ui->lbUser->setText("");
    ui->cbId->setCurrentText("");
    ui->cbId->hidePopup();
}
void DlgLogin::on_btnLogin_clicked()
{
    StUser user;
    if (userList.size() > 0 && curUser <= userList.size()) {
        user = userList[curUser - 1];
    }
    QString userid = ui->cbId->currentText();
    if (!(user.userId == userid)) {
        user.userId = userid;
        user.password = cenc.encrypt(ui->lePwd->text());
        if (ui->cbSavePwd->checkState() == Qt::Checked) {
            user.saveFlag = 1;
        } else {
            user.saveFlag = 0;
        }
    } else {
        user.password = cenc.encrypt(ui->lePwd->text());
        if (ui->cbSavePwd->checkState() == Qt::Checked) {
            user.saveFlag = 1;
        } else {
            user.saveFlag = 0;
        }
    }

    QString username("");
    QList<UserInfo> loginUserList;
    bool isLogin = db_util->getUserList(loginUserList, user.userId, user.username, "");
    if (isLogin) {
        if (loginUserList.size() == 1) {
            loginUser = loginUserList.at(0);
            if (user.password == loginUser.password) {
                username = loginUser.name;
            } else {
                QMessageBox::information(this, QStringLiteral("登录信息"), QStringLiteral("密码错误"));
                return;
            }
        } else {
            QMessageBox::information(this, QStringLiteral("登录信息"), QStringLiteral("无此用户"));
            return;
        }
    } else {
        QMessageBox::information(this, QStringLiteral("登录信息"), QStringLiteral("登录失败，请检查服务器设置"));
        return;
    }
    user.username = username;
    if (user.saveFlag == 0) {
        user.password = "";
    }
    if (userList.size() > 0 && curUser <= userList.size()) {
        bool existFlag = false;
        for (int i=0; i<userList.size(); i++) {
            //若有相同先看是否是当前选择的user
            if (userList[i].userId == user.userId) {
                existFlag = true;
                if (curUser-1 == i) {
                    userList[curUser - 1].password = user.password;
                    userList[curUser - 1].saveFlag = user.saveFlag;
                    set.setUserList(userList);
                    set.setUserIndex(curUser);
                } else {
                    //若不是则改变当前的选择
                    userList[i].password = user.password;
                    userList[i].saveFlag = user.saveFlag;
                    set.setUserList(userList);
                    set.setUserIndex(i);
                }
                break;
            }
        }
        //若无相同则添加
        if (!existFlag) {
            userList.append(user);
            set.setUserList(userList);
            set.setUserIndex(userList.size());
        }
    } else {
        //为空时需要添加
        userList.append(user);
        set.setUserList(userList);
        set.setUserIndex(userList.size());
    }
    emit doLogin(loginUser);
    close();
}

void DlgLogin::on_btnQuit_clicked()
{
    close();
}

void DlgLogin::on_btnServer_clicked()
{
    int tmpH;
    if (showServer) {
        tmpH = bigH;
    } else {
        tmpH = smallH;
    }
    showServer = !showServer;
    setFixedSize(width, tmpH);
}
void DlgLogin::on_btnAdd_clicked()
{
    //传参给dlg的index为 -1；
    dlg_sv = new DlgServer(&dbList, -1, curDb);
    connect(dlg_sv, &DlgServer::dbInfoSaved, this, &DlgLogin::shouldReloadSvTable);
    connect(dlg_sv, &DlgServer::dbInfoSavedAndSelected, this, &DlgLogin::shouldReloadSvTable);
    dlg_sv->exec();
}

void DlgLogin::on_btnSelect_clicked()
{
    int i = ui->tvServer->selectedItems().size();
    if (i > 0) {
        int row_index = ui->tvServer->currentRow();
        curDb = row_index + 1;
        shouldReloadSvTable(dbList, curDb);
    }
}

void DlgLogin::on_btnCancel_clicked()
{
    showServer = !showServer;
    setFixedSize(width, smallH);
}

void DlgLogin::show_sv_custom_context_menu(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(this);

    QAction *add_ac = new QAction(QStringLiteral("添加服务器"), NULL);
    connect(add_ac, &QAction::triggered, this, &DlgLogin::addServer);
    QAction *sel_ac = new QAction(QStringLiteral("选择服务器"), NULL);
    connect(sel_ac, &QAction::triggered, this, &DlgLogin::selectServer);
    QAction *edit_ac = new QAction(QStringLiteral("编辑服务器"), NULL);
    connect(edit_ac, &QAction::triggered, this, &DlgLogin::editServer);
    QAction *del_ac = new QAction(QStringLiteral("删除服务器"), NULL);
    connect(del_ac, &QAction::triggered, this, &DlgLogin::deleteServer);

    popMenu->addAction(add_ac);
    curRightSel = ui->tvServer->itemAt(pos);
    if (curRightSel != NULL) {
        popMenu->addAction(sel_ac);
        popMenu->addAction(edit_ac);
        popMenu->addAction(del_ac);
    }
    popMenu->exec(QCursor::pos());

}

void DlgLogin::addServer()
{
    //传参给dlg的index为 -1；
    dlg_sv = new DlgServer(&dbList, -1, curDb);
    connect(dlg_sv, &DlgServer::dbInfoSaved, this, &DlgLogin::shouldReloadSvTable);
    connect(dlg_sv, &DlgServer::dbInfoSavedAndSelected, this, &DlgLogin::shouldReloadSvTable);
    dlg_sv->exec();
}

void DlgLogin::selectServer()
{
    if (ui->tvServer->selectedItems().size() > 0) {
        int row_index = curRightSel->row();
        curDb = row_index + 1;
        shouldReloadSvTable(dbList, curDb);
    }
}

void DlgLogin::editServer()
{
    if (ui->tvServer->rowCount() > 0) {
        int row_index = curRightSel->row();
        //传参给dlg的index为row + 1；
        dlg_sv = new DlgServer(&dbList, row_index + 1, curDb);
        connect(dlg_sv, &DlgServer::dbInfoSaved, this, &DlgLogin::shouldReloadSvTable);
        connect(dlg_sv, &DlgServer::dbInfoSavedAndSelected, this, &DlgLogin::shouldReloadSvTable);
        dlg_sv->exec();
    }
}

void DlgLogin::deleteServer()
{
    int result = QMessageBox::information(this, QStringLiteral("删除"), QStringLiteral("确认要删除服务器信息？"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (result != QMessageBox::Yes) {
        return;
    }
    if (ui->tvServer->rowCount() > 0) {
        int row_index = curRightSel->row();
        dbList.removeAt(row_index);
        if (row_index == curDb-1) {
            curDb = 0;
        }
        if (row_index < curDb-1) {
            curDb = curDb - 1;
        }
        shouldReloadSvTable(dbList, curDb);
    }
}

void DlgLogin::shouldReloadSvTable(QList<StDb>& dbList, int curIndex)
{
    if (dbList.size() >= 0) {
        ui->tvServer->clearContents();
        ui->tvServer->setRowCount(dbList.size());
        for (int i=0; i<dbList.size(); i++) {
            StDb db_info = dbList.at(i);
            QTableWidgetItem *item = new QTableWidgetItem(db_info.host);
            ui->tvServer->setItem(i,1,item);
            item = new QTableWidgetItem(QString::number(db_info.port));
            ui->tvServer->setItem(i,2,item);
            item = new QTableWidgetItem(db_info.user);
            ui->tvServer->setItem(i,3,item);
            item = new QTableWidgetItem(db_info.name);
            ui->tvServer->setItem(i,4,item);
        }
    }
    if (curIndex > 0) {
        StDb db_cur = dbList.at(curIndex - 1);
        bool bo;
        emit editDbInfo(db_cur, bo);
        if (bo) {
            QIcon icon(QPixmap(":/select").scaled(30,30));
            QTableWidgetItem *item = new QTableWidgetItem(icon, "");
            ui->tvServer->setItem(curIndex-1,0,item);
            set.setDbIndex(curIndex);
            curDb = set.getDbIndex();
        } else {
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("数据库连接失败！"),
                QMessageBox::Yes, QMessageBox::Yes);
        }
    }
    if (curIndex == 0) {
        set.setDbIndex(curIndex);
        curDb = set.getDbIndex();
    }

    ui->tvServer->repaint();
    set.setDbList(dbList);
    set.getDbList(this->dbList);
}

