#include "clientinfomng.h"
#include "ui_clientinfomng.h"

ClientInfoMng::ClientInfoMng(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoMng)
{
    ui->setupUi(this);
}

ClientInfoMng::~ClientInfoMng()
{
    delete ui;
}

ClientInfoMng::ClientInfoMng(DbUtil *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoMng)
{
    ui->setupUi(this);

    model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("客户代码（资金账号）")));
    model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("客户姓名")));
    model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("客户类别")));
    model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("开户日期")));
    ui->tvIns->setModel(&model);
    ui->tvIns->show();
    model.setColumnCount(4);
    model.setRowCount(0);

    ui->btnGoPre->setEnabled(false);
    ui->btnGoFirst->setEnabled(false);
    ui->btnGoLast->setEnabled(false);
    ui->btnGoNext->setEnabled(false);
    ui->btnGoNum->setEnabled(false);

    connect(ui->tvIns, &QTableView::customContextMenuRequested, this, &ClientInfoMng::show_custom_context_menu);
    this->db_util = db;

    QRegExp num_rx("^\\d+$");
    ui->lPageNum->setValidator(new QRegExpValidator(num_rx, this));
    on_btnClear_clicked();
    loadInstanceTable("", "", ui->de_start->date(), ui->de_end->date(), 1);
}

void ClientInfoMng::closeEvent(QCloseEvent *e)
{
    hide();
    QWidget *p = (QWidget *)parent();
    p->hide();
    e->ignore();
}

void ClientInfoMng::setInstanceToRow(int row, ClientInfo &ins)
{
    QModelIndex index;
    index = model.index(row, 0, QModelIndex());
    model.setData(index, QVariant(ins.id));
    index = model.index(row, 1, QModelIndex());
    model.setData(index, QVariant(ins.name));
    index = model.index(row, 2, QModelIndex());
    QString cl_type;
    if (ins.kind == 0) {
        cl_type = QStringLiteral("个人户");
    } else if (ins.kind == 1) {
        cl_type = QStringLiteral("法人户");
    } else {
        cl_type = QStringLiteral("未选择");
    }
    model.setData(index, QVariant(cl_type));
    index = model.index(row, 3, QModelIndex());
    QString oStr = ins.openDate.toString(DB_DATE_FORMATE);
    model.setData(index, QVariant(oStr));
}

void ClientInfoMng:: loadInstanceTable(QString id, QString name, QDate start, QDate end, int page)
{
    ui->btnGoPre->setEnabled(true);
    ui->btnGoFirst->setEnabled(true);
    ui->btnGoLast->setEnabled(true);
    ui->btnGoNext->setEnabled(true);
    ui->btnGoNum->setEnabled(true);
    curPageNum = page;
    pageNum = db_util->getClientInfoListPage(id, name, start, end);
    QString str = QStringLiteral("共") + QString::number(pageNum) +  QStringLiteral("页");
    ui->l_count->setText(str);
    ui->lPageNum->setText(QString::number(page));
    db_util->getClientInfoList(insList, id, name, start, end, page);
    model.setRowCount(insList.size());
    for (int i=0; i<insList.size(); i++) {
        ClientInfo instance = insList.at(i);
        setInstanceToRow(i, instance);
    }
    if (curPageNum <= 1) {
        ui->btnGoPre->setEnabled(false);
    } else {
        ui->btnGoPre->setEnabled(true);
    }
    if (curPageNum >= pageNum) {
        ui->btnGoNext->setEnabled(false);
    } else {
        ui->btnGoNext->setEnabled(true);
    }
}

void ClientInfoMng::show_custom_context_menu(const QPoint &pos)
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
    connect(popMenu, &QMenu::triggered, this, &ClientInfoMng::on_cus_menu);
    popMenu->exec(QCursor::pos());
}

void ClientInfoMng::on_cus_menu(QAction *ac)
{
    int row = curRow;
    if (row >=0) {
        curIns.id = model.item(row, 0)->text();
        curIns.name = model.item(row, 1)->text();
        QString cl_type_str = model.item(row, 2)->text();
        if (cl_type_str == QStringLiteral("个人户")) {
            curIns.kind = 0;
        } else if (cl_type_str == QStringLiteral("法人户")) {
            curIns.kind = 1;
        } else {
            curIns.kind = -1;
        }
        QString op_date_str = model.item(row, 3)->text();
        curIns.openDate = QDate::fromString(op_date_str, DB_DATE_FORMATE);
    }

    if (ac->text() == QStringLiteral("选择记录")) {
        emit sendInfoToRelaMng(curIns);
    }
    if (ac->text() == QStringLiteral("添加记录")) {
        ClientInfo ins;
        pop = new ClientInfoPop(0, ins);
        connect(pop, &ClientInfoPop::instanceSaved, this, &ClientInfoMng::opra_db);
        pop->show();
    }
    if (ac->text() == QStringLiteral("修改记录")) {
        int result = QMessageBox::information(this, QStringLiteral("修改警告"), QStringLiteral("修改该记录将会修改相关的客户关系,确认要修改？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        pop = new ClientInfoPop(1, curIns);
        connect(pop, &ClientInfoPop::instanceSaved, this, &ClientInfoMng::opra_db);
        pop->show();
    }
    if (ac->text() == QStringLiteral("删除记录")) {
        int result = QMessageBox::information(this, QStringLiteral("删除警告"), QStringLiteral("删除该记录将会删除相关的客户关系,确认要删除？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        bool ok = db_util->deleteClientInfo(curIns);
        if (ok) {
            model.removeRow(row, QModelIndex());
            emit relaInfoDelete(curIns);
        }
    }
}

void ClientInfoMng::opra_db(ClientInfo &instance, int save_type)
{
    int row = curRow;
    if (row < 0) {
       bool ok = db_util->insertClientInfo(instance);
       if (ok) {
           int a = model.rowCount();
           model.insertRow(a, QModelIndex());
           setInstanceToRow(a, instance);
       }
    } else {
        //添加
        if (save_type == 0) {
            bool ok = db_util->insertClientInfo(instance);
            if (ok) {
                model.insertRow(row+1, QModelIndex());
                setInstanceToRow(row+1, instance);
            }
        }
        //编辑
        if (save_type == 1) {
            bool ok = db_util->updateClientInfo(instance, curIns);
            if (ok) {
                setInstanceToRow(row, instance);
                emit relaInfoUpdate(instance, curIns);
            }
        }
    }
}

void ClientInfoMng::on_btnQuery_clicked()
{
    QString id = ui->leqryId->text();
    QString name = ui->leqryName->text();
    QDate start = ui->de_start->date();
    QDate end = ui->de_end->date();
    loadInstanceTable(id, name, start, end, 1);
}

void ClientInfoMng::on_btnClear_clicked()
{
    ui->leqryId->setText("");
    ui->leqryName->setText("");
    ui->de_start->setDate(QDate::fromString("1999-01-01", DB_DATE_FORMATE));
    ui->de_end->setDate(QDate::currentDate());
}

void ClientInfoMng::on_btnGoFirst_clicked()
{
    curPageNum = 1;
    QString id = ui->leqryId->text();
    QString name = ui->leqryName->text();
    QDate start = ui->de_start->date();
    QDate end = ui->de_end->date();
    loadInstanceTable(id, name, start, end, curPageNum);
}

void ClientInfoMng::on_btnGoPre_clicked()
{
    curPageNum = curPageNum - 1;
    QString id = ui->leqryId->text();
    QString name = ui->leqryName->text();
    QDate start = ui->de_start->date();
    QDate end = ui->de_end->date();
    loadInstanceTable(id, name, start, end, curPageNum);
}

void ClientInfoMng::on_btnGoNext_clicked()
{
    curPageNum = curPageNum + 1;
    QString id = ui->leqryId->text();
    QString name = ui->leqryName->text();
    QDate start = ui->de_start->date();
    QDate end = ui->de_end->date();
    loadInstanceTable(id, name, start, end, curPageNum);
}

void ClientInfoMng::on_btnGoLast_clicked()
{
    curPageNum = pageNum;
    QString id = ui->leqryId->text();
    QString name = ui->leqryName->text();
    QDate start = ui->de_start->date();
    QDate end = ui->de_end->date();
    loadInstanceTable(id, name, start, end, curPageNum);
}

void ClientInfoMng::on_btnGoNum_clicked()
{
    int page = ui->lPageNum->text().toInt();
    if (page > pageNum || page <= 0) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("不在页数范围"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        curPageNum = page;
        QString id = ui->leqryId->text();
        QString name = ui->leqryName->text();
        QDate start = ui->de_start->date();
        QDate end = ui->de_end->date();
        loadInstanceTable(id, name, start, end, curPageNum);
    }
}
