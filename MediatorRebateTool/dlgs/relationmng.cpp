#include "relationmng.h"
#include "ui_relationmng.h"

RelationMng::RelationMng(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RelationMng)
{
    ui->setupUi(this);
}

RelationMng::~RelationMng()
{
    delete ui;
}

RelationMng::RelationMng(DbUtil *db, AgentInfoMng *agentMng, RcmInfoMng *rcmMng, BrokerInfoMng *brokerMng, ClientInfoMng *clientMng, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RelationMng)
{
    ui->setupUi(this);

    connect(ui->tvIns, &QTableView::customContextMenuRequested, this, &RelationMng::show_custom_context_menu);

    ui->cm_rela_type->addItem(QStringLiteral("居间人"));
    ui->cm_rela_type->addItem(QStringLiteral("代理人"));
    ui->cm_rela_type->addItem(QStringLiteral("开发人"));
    ui->cm_rela_type->setCurrentIndex(0);
    curRelaType = 0;

    on_btnClear_clicked();

    this->db_util = db;
    this->agentMng = agentMng;
    this->rcmMng = rcmMng;
    this->brokerMng = brokerMng;
    this->clientMng = clientMng;

    connect(this->brokerMng, &BrokerInfoMng::sendInfoToRelaMng, this, &RelationMng::getBrokerInfo);
    connect(this->agentMng, &AgentInfoMng::sendInfoToRelaMng, this, &RelationMng::getAgentInfo);
    connect(this->rcmMng, &RcmInfoMng::sendInfoToRelaMng, this, &RelationMng::getRcmInfo);
    connect(this->clientMng, &ClientInfoMng::sendInfoToRelaMng, this, &RelationMng::getClientInfo);

    connect(this->brokerMng, &BrokerInfoMng::relaInfoDelete, this, &RelationMng::delRelaBroker);
    connect(this->agentMng, &AgentInfoMng::relaInfoDelete, this, &RelationMng::delRelaAgent);
    connect(this->rcmMng, &RcmInfoMng::relaInfoDelete, this, &RelationMng::delRelaRcm);
    connect(this->clientMng, &ClientInfoMng::relaInfoDelete, this, &RelationMng::delRelaClient);

    connect(this->brokerMng, &BrokerInfoMng::relaInfoUpdate, this, &RelationMng::updateRelaBroker);
    connect(this->agentMng, &AgentInfoMng::relaInfoUpdate, this, &RelationMng::updateRelaAgent);
    connect(this->rcmMng, &RcmInfoMng::relaInfoUpdate, this, &RelationMng::updateRelaRcm);
    connect(this->clientMng, &ClientInfoMng::relaInfoUpdate, this, &RelationMng::updateRelaClient);


    QRegExp num_rx("^\\d+$");
    ui->lPageNum->setValidator(new QRegExpValidator(num_rx, this));

}

void RelationMng::closeEvent(QCloseEvent *e)
{
    hide();
    QWidget *p = (QWidget *)parent();
    p->hide();
    e->ignore();
}

void RelationMng::on_cm_rela_type_currentIndexChanged(int index)
{
    curRelaType = index;
    on_btnClear_clicked();
}

void RelationMng::getClientInfo(ClientInfo &info)
{
    curClientInfo.id = info.id;
    curClientInfo.name = info.name;
    curClientInfo.kind = info.kind;
    ui->la_client->setText(curClientInfo.name);
    showMaximized();
}

void RelationMng::getAgentInfo(AgentInfo &info)
{
    curAgentInfo.id = info.id;
    curAgentInfo.name = info.name;
    ui->la_rela->setText(curAgentInfo.name);
    showMaximized();
}

void RelationMng::getBrokerInfo(BrokerInfo &info)
{
    curBrokerInfo.id = info.id;
    curBrokerInfo.name = info.name;
    curBrokerInfo.kind = info.kind;
    ui->la_rela->setText(curBrokerInfo.name);
    showMaximized();
}

void RelationMng::getRcmInfo(RcmInfo &info)
{
    curRcmInfo.id = info.id;
    curRcmInfo.name = info.name;
    ui->la_rela->setText(curRcmInfo.name);
    showMaximized();
}

void RelationMng::setRcmRelaToRow(int row, RcmRela &ins)
{
    QModelIndex index;
    index = model.index(row, 0, QModelIndex());
    model.setData(index, QVariant(ins.clientId));
    index = model.index(row, 1, QModelIndex());
    model.setData(index, QVariant(ins.clientName));
    index = model.index(row, 2, QModelIndex());
    model.setData(index, QVariant(ins.rcmId));
    index = model.index(row, 3, QModelIndex());
    model.setData(index, QVariant(ins.rcmName));
    index = model.index(row, 4, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.ratio, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 5, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.share, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 6, QModelIndex());
    QString startStr = ins.startDate.toString(DB_DATE_FORMATE);
    model.setData(index, QVariant(startStr));
    index = model.index(row, 7, QModelIndex());
    QString endStr = ins.endDate.toString(DB_DATE_FORMATE);
    model.setData(index, QVariant(endStr));
}

void RelationMng::setAgentRelaToRow(int row, AgentRela &ins)
{
    QModelIndex index;
    index = model.index(row, 0, QModelIndex());
    model.setData(index, QVariant(ins.clientId));
    index = model.index(row, 1, QModelIndex());
    model.setData(index, QVariant(ins.clientName));
    index = model.index(row, 2, QModelIndex());
    model.setData(index, QVariant(ins.agentId));
    index = model.index(row, 3, QModelIndex());
    model.setData(index, QVariant(ins.agentName));
    index = model.index(row, 4, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.ratio, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 5, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.share, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 6, QModelIndex());
    QString startStr = ins.startDate.toString(DB_DATE_FORMATE);
    model.setData(index, QVariant(startStr));
    index = model.index(row, 7, QModelIndex());
    QString endStr = ins.endDate.toString(DB_DATE_FORMATE);
    model.setData(index, QVariant(endStr));
    index = model.index(row, 8, QModelIndex());
    model.setData(index, QVariant(ins.ibName));
}

void RelationMng::setBrokerRelaToRow(int row, BrokerRela &ins)
{
    QModelIndex index;
    index = model.index(row, 0, QModelIndex());
    model.setData(index, QVariant(ins.clientId));
    index = model.index(row, 1, QModelIndex());
    model.setData(index, QVariant(ins.clientName));
    index = model.index(row, 2, QModelIndex());
    model.setData(index, QVariant(ins.brokerId));
    index = model.index(row, 3, QModelIndex());
    model.setData(index, QVariant(ins.brokerName));
    index = model.index(row, 4, QModelIndex());
    model.setData(index, QVariant(ins.sectionNum));
    index = model.index(row, 5, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.sectionStart, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 6, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.sectionEnd, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 7, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.ratio, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 8, QModelIndex());
    QString startStr = ins.startDate.toString(DB_DATE_FORMATE);
    model.setData(index, QVariant(startStr));
    index = model.index(row, 9, QModelIndex());
    QString endStr = ins.endDate.toString(DB_DATE_FORMATE);
    model.setData(index, QVariant(endStr));
}

void RelationMng::loadInstanceTable(int relaType, QString clientId, QString relaId, int page)
{
    ui->btnGoFirst->setEnabled(true);
    ui->btnGoLast->setEnabled(true);
    ui->btnGoNum->setEnabled(true);
    ui->btnGoNext->setEnabled(true);
    ui->btnGoPre->setEnabled(true);
    if (!clientId.isEmpty() && !relaId.isEmpty()) {
        enableCusMenu = true;
    } else {
        enableCusMenu = false;
    }

    if (relaType == 0) {
        curPageNum = page;
        pageNum = db_util->getBrokerRelaListPage(clientId, relaId);
        QString str = QStringLiteral("共") + QString::number(pageNum) +  QStringLiteral("页");
        ui->l_count->setText(str);
        ui->lPageNum->setText(QString::number(curPageNum));

        model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("客户代码（资金账号）")));
        model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("客户姓名")));
        model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("居间人代码")));
        model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("居间人名称")));
        model.setHorizontalHeaderItem(4, new QStandardItem(QStringLiteral("方案区间编号")));
        model.setHorizontalHeaderItem(5, new QStandardItem(QStringLiteral("区间起始值")));
        model.setHorizontalHeaderItem(6, new QStandardItem(QStringLiteral("区间结束值")));
        model.setHorizontalHeaderItem(7, new QStandardItem(QStringLiteral("提成比例")));
        model.setHorizontalHeaderItem(8, new QStandardItem(QStringLiteral("开始时间")));
        model.setHorizontalHeaderItem(9, new QStandardItem(QStringLiteral("结束时间")));
        model.setColumnCount(10);
        ui->tvIns->setModel(&model);
        ui->tvIns->show();
        db_util->getBrokerRelaList(curBrokerRelaList, clientId, relaId, page);
        model.setRowCount(curBrokerRelaList.size());
        for (int i=0; i<curBrokerRelaList.size(); i++) {
            BrokerRela ins = curBrokerRelaList.at(i);
            setBrokerRelaToRow(i, ins);
        }

    } else if (relaType == 1) {
        curPageNum = page;
        pageNum = db_util->getAgentRelaListPage(clientId, relaId);
        QString str = QStringLiteral("共") + QString::number(pageNum) +  QStringLiteral("页");
        ui->l_count->setText(str);
        ui->lPageNum->setText(QString::number(curPageNum));

        model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("客户代码（资金账号）")));
        model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("客户姓名")));
        model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("代理人代码")));
        model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("代理人名称")));
        model.setHorizontalHeaderItem(4, new QStandardItem(QStringLiteral("提成比例")));
        model.setHorizontalHeaderItem(5, new QStandardItem(QStringLiteral("提成分成")));
        model.setHorizontalHeaderItem(6, new QStandardItem(QStringLiteral("开始时间")));
        model.setHorizontalHeaderItem(7, new QStandardItem(QStringLiteral("结束时间")));
        model.setHorizontalHeaderItem(8, new QStandardItem(QStringLiteral("IB部门")));
        model.setColumnCount(9);
        ui->tvIns->setModel(&model);
        ui->tvIns->show();
        db_util->getAgentRelaList(curAgentRelaList, clientId, relaId, page);
        model.setRowCount(curAgentRelaList.size());
        for (int i=0; i<curAgentRelaList.size(); i++) {
            AgentRela ins = curAgentRelaList[i];
            setAgentRelaToRow(i, ins);
        }

    } else if (relaType == 2) {
        curPageNum = page;
        pageNum = db_util->getRcmRelaListPage(clientId, relaId);
        QString str = QStringLiteral("共") + QString::number(pageNum) +  QStringLiteral("页");
        ui->l_count->setText(str);
        ui->lPageNum->setText(QString::number(curPageNum));

        model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("客户代码（资金账号）")));
        model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("客户姓名")));
        model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("开发人代码")));
        model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("开发人名称")));
        model.setHorizontalHeaderItem(4, new QStandardItem(QStringLiteral("提成比例")));
        model.setHorizontalHeaderItem(5, new QStandardItem(QStringLiteral("提成分成")));
        model.setHorizontalHeaderItem(6, new QStandardItem(QStringLiteral("开始时间")));
        model.setHorizontalHeaderItem(7, new QStandardItem(QStringLiteral("结束时间")));
        model.setColumnCount(8);
        ui->tvIns->setModel(&model);
        ui->tvIns->show();
        db_util->getRcmRelaList(curRcmRelaList, clientId, relaId, page);
        model.setRowCount(curRcmRelaList.size());
        for (int i=0; i<curRcmRelaList.size(); i++) {
            RcmRela ins = curRcmRelaList[i];
            setRcmRelaToRow(i, ins);
        }

    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请选择关系类别"),
            QMessageBox::Yes, QMessageBox::Yes);
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

void RelationMng::show_custom_context_menu(const QPoint &pos)
{
    if (enableCusMenu) {
        if (curRelaType == 0) {
            QMenu *popMenu = new QMenu(this);
            popMenu->addAction(QStringLiteral("编辑客户居间人关系"));
            connect(popMenu, &QMenu::triggered, this, &RelationMng::on_cus_menu);
            popMenu->exec(QCursor::pos());

        } else if (curRelaType == 1) {
            QMenu *popMenu = new QMenu(this);
            QModelIndex cur = ui->tvIns->indexAt(pos);
            curRow = cur.row();
            if (curAgentRelaList.size() == 0) {
                popMenu->addAction(QStringLiteral("添加客户代理人关系"));

            } else if (curAgentRelaList.size() == 1){
                popMenu->addAction(QStringLiteral("修改客户代理人关系"));
                popMenu->addAction(QStringLiteral("删除客户代理人关系"));

            } else {
                QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("数据库中的关系数据有重复或其他问题"),
                    QMessageBox::Yes, QMessageBox::Yes);
            }
            connect(popMenu, &QMenu::triggered, this, &RelationMng::on_cus_menu);
            popMenu->exec(QCursor::pos());

        } else if (curRelaType == 2) {
            QMenu *popMenu = new QMenu(this);
            QModelIndex cur = ui->tvIns->indexAt(pos);
            curRow = cur.row();
            if (curRcmRelaList.size() == 0) {
                popMenu->addAction(QStringLiteral("添加客户开发人关系"));

            } else if (curRcmRelaList.size() == 1){
                popMenu->addAction(QStringLiteral("修改客户开发人关系"));
                popMenu->addAction(QStringLiteral("删除客户开发人关系"));

            } else {
                QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("数据库中的关系数据有重复或其他问题"),
                    QMessageBox::Yes, QMessageBox::Yes);
            }
            connect(popMenu, &QMenu::triggered, this, &RelationMng::on_cus_menu);
            popMenu->exec(QCursor::pos());

        } else {
        }

    }
}

void RelationMng::on_cus_menu(QAction *ac)
{
    int row = curRow;

    if (ac->text() == QStringLiteral("编辑客户居间人关系")) {
        brokerPop = new BrokerRelaPop(curClientInfo, curBrokerInfo, curBrokerRelaList);
        connect(brokerPop, &BrokerRelaPop::brokerRelasSaved, this, &RelationMng::opra_broker_db);
        brokerPop->show();
    }
    if (ac->text() == QStringLiteral("添加客户代理人关系")) {
        AgentRela rela;
        rela.clientId = curClientInfo.id;
        rela.clientName = curClientInfo.name;
        rela.agentId = curAgentInfo.id;
        rela.agentName = curAgentInfo.name;
        rela.ratio = 0;
        rela.share = 0;
        rela.startDate = QDate::currentDate();
        rela.endDate = QDate::currentDate();

        agentPop = new AgentRelaPop(0, rela);
        connect(agentPop, &AgentRelaPop::relaSaved, this, &RelationMng::opra_agent_db);
        agentPop->show();
    }
    if (ac->text() == QStringLiteral("修改客户代理人关系")) {
        agentPop = new AgentRelaPop(1, curAgentRelaList[0]);
        connect(agentPop, &AgentRelaPop::relaSaved, this, &RelationMng::opra_agent_db);
        agentPop->show();
    }
    if (ac->text() == QStringLiteral("删除客户代理人关系")) {
        int result = QMessageBox::information(this, QStringLiteral("删除"), QStringLiteral("确认要删除？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        bool ok = db_util->deleteAgentRela(curAgentRelaList[0]);
        if (ok) {
            model.removeRow(row, QModelIndex());
        }
    }
    if (ac->text() == QStringLiteral("添加客户开发人关系")) {
        RcmRela rela;
        rela.clientId = curClientInfo.id;
        rela.clientName = curClientInfo.name;
        rela.rcmId = curRcmInfo.id;
        rela.rcmName = curRcmInfo.name;
        rela.ratio = 0;
        rela.share = 0;
        rela.startDate = QDate::currentDate();
        rela.endDate = QDate::currentDate();

        rcmPop = new RcmRelaPop(0, rela);
        connect(rcmPop, &RcmRelaPop::relaSaved, this, &RelationMng::opra_rcm_db);
        rcmPop->show();
    }
    if (ac->text() == QStringLiteral("修改客户开发人关系")) {
        rcmPop = new RcmRelaPop(1, curRcmRelaList[0]);
        connect(rcmPop, &RcmRelaPop::relaSaved, this, &RelationMng::opra_rcm_db);
        rcmPop->show();
    }
    if (ac->text() == QStringLiteral("删除客户开发人关系")) {
        int result = QMessageBox::information(this, QStringLiteral("删除"), QStringLiteral("确认要删除？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        bool ok = db_util->deleteRcmRela(curRcmRelaList[0]);
        if (ok) {
            model.removeRow(row, QModelIndex());
        }
    }
}

void RelationMng::opra_rcm_db(RcmRela &ins, int save_type)
{
    //添加
    if (save_type == 0) {
        bool ok = db_util->insertRcmRela(ins);
        if (ok) {
            loadInstanceTable(curRelaType, curClientInfo.id, curRcmInfo.id, 1);
        }
    }
    //编辑
    if (save_type == 1) {
        bool ok = db_util->updateRcmRela(ins, curRcmRelaList[0]);
        if (ok) {
            loadInstanceTable(curRelaType, curClientInfo.id, curRcmInfo.id, 1);
        }
    }
}

void RelationMng::opra_agent_db(AgentRela &ins, int save_type)
{
    //添加
    if (save_type == 0) {
        bool ok = db_util->insertAgentRela(ins);
        if (ok) {
            loadInstanceTable(curRelaType, curClientInfo.id, curAgentInfo.id, 1);
        }
    }
    //编辑
    if (save_type == 1) {
        bool ok = db_util->updateAgentRela(ins, curAgentRelaList[0]);
        if (ok) {
            loadInstanceTable(curRelaType, curClientInfo.id, curAgentInfo.id, 1);
        }
    }

}

void RelationMng::opra_broker_db(QList<BrokerRela> &list)
{
    for (int i=0; i<curBrokerRelaList.size(); i++) {
        db_util->deleteBrokerRela(curBrokerRelaList[i]);
    }
    for (int i=0; i<list.size(); i++) {
        db_util->insertBrokerRela(list[i]);
    }
    loadInstanceTable(curRelaType, curClientInfo.id, curBrokerInfo.id, 1);
}

void RelationMng::on_btnRela_clicked()
{
    if (curRelaType == 0) {
        this->brokerMng->setVisible(true);
        this->brokerMng->showMaximized();
    } else if (curRelaType == 1) {
        this->agentMng->setVisible(true);
        this->agentMng->showMaximized();
    } else if (curRelaType == 2) {
        this->rcmMng->setVisible(true);
        this->rcmMng->showMaximized();
    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("请选择关系类别"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void RelationMng::on_btnClient_clicked()
{
    this->clientMng->setVisible(true);
    this->clientMng->showMaximized();
}


void RelationMng::on_btnClear_clicked()
{
    curClientInfo.id = "";
    curClientInfo.name = "";
    curAgentInfo.id = "";
    curAgentInfo.name = "";
    curBrokerInfo.id = "";
    curBrokerInfo.name = "";
    curRcmInfo.id = "";
    curRcmInfo.name = "";
    ui->la_client->setText("");
    ui->la_rela->setText("");
    curRcmRelaList.clear();
    curAgentRelaList.clear();
    curBrokerRelaList.clear();
    enableCusMenu = false;
    model.clear();
    ui->tvIns->setModel(&model);

    ui->btnGoFirst->setEnabled(false);
    ui->btnGoLast->setEnabled(false);
    ui->btnGoNum->setEnabled(false);
    ui->btnGoNext->setEnabled(false);
    ui->btnGoPre->setEnabled(false);
}

void RelationMng::on_btnQuery_clicked()
{
    curPageNum = 1;
    if (curRelaType == 0) {
        loadInstanceTable(0, curClientInfo.id, curBrokerInfo.id, 1);

    } else if (curRelaType == 1) {
        loadInstanceTable(1, curClientInfo.id, curAgentInfo.id, 1);

    } else if (curRelaType == 2) {
        loadInstanceTable(2, curClientInfo.id, curRcmInfo.id, 1);

    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("请选择关系类别"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void RelationMng::on_btnGoFirst_clicked()
{
    curPageNum = 1;
    ui->lPageNum->setText(QString::number(curPageNum));
    if (curRelaType == 0) {
        loadInstanceTable(0, curClientInfo.id, curBrokerInfo.id, 1);

    } else if (curRelaType == 1) {
        loadInstanceTable(1, curClientInfo.id, curAgentInfo.id, 1);

    } else if (curRelaType == 2) {
        loadInstanceTable(2, curClientInfo.id, curRcmInfo.id, 1);

    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("请选择关系类别"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void RelationMng::on_btnGoPre_clicked()
{
    curPageNum = curPageNum - 1;
    if (curRelaType == 0) {
        loadInstanceTable(0, curClientInfo.id, curBrokerInfo.id, curPageNum);

    } else if (curRelaType == 1) {
        loadInstanceTable(1, curClientInfo.id, curAgentInfo.id, curPageNum);

    } else if (curRelaType == 2) {
        loadInstanceTable(2, curClientInfo.id, curRcmInfo.id, curPageNum);

    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("请选择关系类别"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void RelationMng::on_btnGoNext_clicked()
{
    curPageNum = curPageNum + 1;
    if (curRelaType == 0) {
        loadInstanceTable(0, curClientInfo.id, curBrokerInfo.id, curPageNum);

    } else if (curRelaType == 1) {
        loadInstanceTable(1, curClientInfo.id, curAgentInfo.id, curPageNum);

    } else if (curRelaType == 2) {
        loadInstanceTable(2, curClientInfo.id, curRcmInfo.id, curPageNum);

    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("请选择关系类别"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void RelationMng::on_btnGoLast_clicked()
{
    curPageNum = pageNum;
    if (curRelaType == 0) {
        loadInstanceTable(0, curClientInfo.id, curBrokerInfo.id, curPageNum);

    } else if (curRelaType == 1) {
        loadInstanceTable(1, curClientInfo.id, curAgentInfo.id, curPageNum);

    } else if (curRelaType == 2) {
        loadInstanceTable(2, curClientInfo.id, curRcmInfo.id, curPageNum);

    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("请选择关系类别"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void RelationMng::on_btnGoNum_clicked()
{
    int page = ui->lPageNum->text().toInt();
    if (page > pageNum || page <= 0) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("不在页数范围"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        curPageNum = page;
        if (curRelaType == 0) {
            loadInstanceTable(0, curClientInfo.id, curBrokerInfo.id, curPageNum);

        } else if (curRelaType == 1) {
            loadInstanceTable(1, curClientInfo.id, curAgentInfo.id, curPageNum);

        } else if (curRelaType == 2) {
            loadInstanceTable(2, curClientInfo.id, curRcmInfo.id, curPageNum);

        } else {
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("请选择关系类别"),
                QMessageBox::Yes, QMessageBox::Yes);
        }
    }
}

void RelationMng::delRelaAgent(AgentInfo &info)
{
    this->db_util->deleteAgentRela("", info.id);
}

void RelationMng::updateRelaAgent(AgentInfo &info, AgentInfo &old)
{
    this->db_util->updateAgentRela(info, old);
}

void RelationMng::delRelaRcm(RcmInfo &info)
{
    this->db_util->deleteRcmRela("", info.id);
}

void RelationMng::updateRelaRcm(RcmInfo &info, RcmInfo &old)
{
    this->db_util->updateRcmRela(info, old);
}

void RelationMng::delRelaBroker(BrokerInfo &info)
{
    this->db_util->deleteBrokerRela("", info.id);
}

void RelationMng::updateRelaBroker(BrokerInfo &info, BrokerInfo &old)
{
    this->db_util->updateBrokerRela(info, old);
}

void RelationMng::delRelaClient(ClientInfo &info)
{
    this->db_util->deleteAgentRela(info.id, "");
    this->db_util->deleteRcmRela(info.id, "");
    this->db_util->deleteBrokerRela(info.id, "");
}

void RelationMng::updateRelaClient(ClientInfo &info, ClientInfo &old)
{
    this->db_util->updateAgentRela(info, old);
    this->db_util->updateRcmRela(info, old);
    this->db_util->updateBrokerRela(info, old);
}

