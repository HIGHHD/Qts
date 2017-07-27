#include "crmexportdlg.h"
#include "ui_crmexportdlg.h"

CrmExportDlg::CrmExportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CrmExportDlg)
{
    ui->setupUi(this);
}

CrmExportDlg::CrmExportDlg(DbUtil *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CrmExportDlg)
{
    ui->setupUi(this);
    ui->cm_rela->addItem(QStringLiteral("居间人"));
    ui->cm_rela->addItem(QStringLiteral("代理人"));
    ui->cm_rela->addItem(QStringLiteral("开发人"));
    ui->cm_rela->setCurrentIndex(0);
    curRelaType = 0;

    ui->list_rela->setSortingEnabled(false);
    ui->list_rela->setColumnCount(1);

    this->db_util = db;
    on_cm_rela_activated(0);
    on_btn_reset_clicked();
}

CrmExportDlg::~CrmExportDlg()
{
    delete ui;
}

void CrmExportDlg::closeEvent(QCloseEvent *e)
{
    hide();
    QWidget *p = (QWidget *)parent();
    p->hide();
    e->ignore();
}

void CrmExportDlg::on_cm_rela_activated(int index)
{
    curRelaType = index;
    infoIdList.clear();

    ui->tvData->clear();
    ui->tvData->setRowCount(0);
    ui->tvData->setColumnCount(0);
    ui->tvData->repaint();
    isGenerate = false;

    ui->list_rela->clear();
    ui->list_rela->setHeaderLabel(ui->cm_rela->currentText());
    db_util->getDeptList(deptList, "", "");
    if (curRelaType == 0) {
        for (int i=0; i<deptList.size(); i++) {
            StDept dept = deptList.at(i);
            QTreeWidgetItem *root = new QTreeWidgetItem(QStringList(dept.name));
            root->setCheckState(0, Qt::Unchecked);
            QList<BrokerInfo> infoList;
            db_util->getBrokerInfoList(infoList, "", "", dept.id);
            if (infoList.size() > 0) {
                for (int j=0; j<infoList.size(); j++) {
                    BrokerInfo info = infoList.at(j);
                    QTreeWidgetItem *item = new QTreeWidgetItem(root, QStringList(info.name + "(" + info.id + ")"));
                    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                    item->setCheckState(0, Qt::Unchecked);
                }
                ui->list_rela->addTopLevelItem(root);
            }
        }
    } else if (curRelaType == 1) {
        for (int i=0; i<deptList.size(); i++) {
            StDept dept = deptList.at(i);
            QTreeWidgetItem *root = new QTreeWidgetItem(QStringList(dept.name));
            root->setCheckState(0, Qt::Unchecked);
            QList<AgentInfo> infoList;
            db_util->getAgentInfoList(infoList, "", "", dept.id);
            if (infoList.size() > 0) {
                for (int j=0; j<infoList.size(); j++) {
                    AgentInfo info = infoList.at(j);
                    QTreeWidgetItem *item = new QTreeWidgetItem(root, QStringList(info.name + "(" + info.id + ")"));
                    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                    item->setCheckState(0, Qt::Unchecked);
                }
                ui->list_rela->addTopLevelItem(root);
            }
        }
    } else if (curRelaType == 2) {
        for (int i=0; i<deptList.size(); i++) {
            StDept dept = deptList.at(i);
            QTreeWidgetItem *root = new QTreeWidgetItem(QStringList(dept.name));
            root->setCheckState(0, Qt::Unchecked);
            QList<RcmInfo> infoList;
            db_util->getRcmInfoList(infoList, "", "", dept.id);
            if (infoList.size() > 0) {
                for (int j=0; j<infoList.size(); j++) {
                    RcmInfo info = infoList.at(j);
                    QTreeWidgetItem *item = new QTreeWidgetItem(root, QStringList(info.name + "(" + info.id + ")"));
                    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                    item->setCheckState(0, Qt::Unchecked);
                }
                ui->list_rela->addTopLevelItem(root);
            }
        }
    } else {}
    ui->list_rela->expandAll();
}

void CrmExportDlg::on_list_rela_itemChanged(QTreeWidgetItem *item, int column)
{
    if(Qt::PartiallyChecked!=item->checkState(0))
        setChildCheckState(item,item->checkState(0));

    if(Qt::PartiallyChecked==item->checkState(0))
        if(!isTopItem(item))
            item->parent()->setCheckState(0,Qt::PartiallyChecked);
}

bool CrmExportDlg::isTopItem(QTreeWidgetItem* item)
{
    if(!item) return false;
    if(!item->parent()) return true;
    return false;
}

void CrmExportDlg::setChildCheckState(QTreeWidgetItem *item, Qt::CheckState cs)
{
    if(!item) return;
    for (int i=0;i<item->childCount();i++)
    {
        QTreeWidgetItem* child=item->child(i);
        if(child->checkState(0)!=cs)
        {
            child->setCheckState(0, cs);
        }
    }
    setParentCheckState(item->parent());
}

void CrmExportDlg::setParentCheckState(QTreeWidgetItem *item)
{
    if(!item) return;
    int selectedCount=0;
    int childCount = item->childCount();
    for (int i=0;i<childCount;i++)
    {
        QTreeWidgetItem* child= item->child(i);
        if(child->checkState(0)==Qt::Checked)
        {
            selectedCount++;
        }
    }

    if(selectedCount == 0) {
        item->setCheckState(0,Qt::Unchecked);
    } else if (selectedCount == childCount) {
        item->setCheckState(0,Qt::Checked);
    } else {
        item->setCheckState(0,Qt::PartiallyChecked);
    }
}

void CrmExportDlg::generateCrmInfo()
{
    brokerCrmList.clear();
    agentCrmList.clear();
    rcmCrmList.clear();
    QDate start_date = ui->de_start->date();
    QDate end_date = ui->de_end->date();

    if (curRelaType == 0) {
        for (int k=0; k<infoIdList.size(); k++) {
            double lcsxf_sum = 0.000;
            double rebate_sum = 0.000;
            QString brokerId = infoIdList.at(k);
            QList<DealInfo> dealList;
            db_util->getDealList(dealList, brokerId, "", start_date, end_date);
            //由于日期不一定是当天，要使用map加总
            QMap<QString, DealInfo> rebateMap;
            for (int i=0; i<dealList.size(); i++) {
                DealInfo dealInfo = dealList.at(i);
                if (rebateMap.contains(dealInfo.clientId)) {
                    DealInfo tmpInfo = rebateMap[dealInfo.clientId];
                    rebateMap[dealInfo.clientId].lc_charge = tmpInfo.lc_charge + dealInfo.lc_charge;
                    rebateMap[dealInfo.clientId].rebate = tmpInfo.rebate + dealInfo.rebate;
                } else {
                    DealInfo tmpInfo;
                    tmpInfo.lc_charge = dealInfo.lc_charge;
                    tmpInfo.rebate = dealInfo.rebate;
                    tmpInfo.clientId = dealInfo.clientId;
                    tmpInfo.clientName = dealInfo.clientName;
                    tmpInfo.brokerName = dealInfo.brokerName;
                    rebateMap.insert(dealInfo.clientId, tmpInfo);
                }
            }
            for (int i=0; i<rebateMap.keys().size(); i++) {
                QString key = rebateMap.keys().at(i);
                DealInfo tmpInfo = rebateMap[key];
                BrokerCrm crm;
                crm.brokerName = tmpInfo.brokerName;
                crm.clientId = tmpInfo.clientId;
                crm.clientName = tmpInfo.clientName;
                crm.j_charge = tmpInfo.lc_charge;
                crm.re_money = tmpInfo.rebate;
                lcsxf_sum += crm.j_charge;
                rebate_sum += crm.re_money;
                brokerCrmList.append(crm);
            }
            if (rebateMap.size() > 0) {
                BrokerCrm crm;
                crm.brokerName = QStringLiteral("小计：");
                crm.j_charge = lcsxf_sum;
                crm.re_money = rebate_sum;
                brokerCrmList.append(crm);
            }
        }
    } else if (curRelaType == 1) {
        for (int k=0; k<infoIdList.size(); k++) {
            double lcsxf_sum = 0.000;
            double rebate_sum = 0.000;
            QList<AgentRela> agentRelaList;
            QString agentId = infoIdList.at(k);
            int page = db_util->getAgentRelaListPage("", agentId);
            for (int i=1; i<=page; i++) {
                QList<AgentRela> tmpList;
                db_util->getAgentRelaList(tmpList, "", agentId, i);
                agentRelaList.append(tmpList);
            }
            QMap<QString, AgentCrm> rebateMap;
            for (int i=0; i<agentRelaList.size(); i++) {
                AgentRela rela = agentRelaList.at(i);
                if (rela.agentId == agentId) {
                    QList<DealInfo> dealList;
                    db_util->getDealList(dealList, "", rela.clientId, start_date, end_date);
                    double sum_equity = 0;
                    int days = 0;
                    for (int j=0; j<dealList.size(); j++) {
                        DealInfo dealInfo = dealList.at(j);
                        if (rela.startDate <= dealInfo.theDate && rela.endDate >= dealInfo.theDate) {
                            QList<FundInfo> fundList;
                            db_util->getFundList(fundList, dealInfo.clientId, dealInfo.theDate,dealInfo.theDate);
                            if (fundList.size() > 0) {
                                sum_equity += fundList.at(0).today_equity;
                                days ++;
                            }
                            if (rebateMap.contains(dealInfo.clientId)) {
                                rebateMap[dealInfo.clientId].b_money = rebateMap[dealInfo.clientId].b_money + dealInfo.rebate;
                                double re_money = (dealInfo.lc_charge - dealInfo.rebate) * (rela.ratio/100) * (rela.share/100);
                                rebateMap[dealInfo.clientId].re_money = rebateMap[dealInfo.clientId].re_money + re_money;
                                rebateMap[dealInfo.clientId].j_charge = rebateMap[dealInfo.clientId].j_charge + dealInfo.lc_charge;
                            } else {
                                AgentCrm crm;
                                crm.agentName = rela.agentName;
                                crm.clientId = rela.clientId;
                                crm.clientName = rela.clientName;
                                crm.re_money = (dealInfo.lc_charge - dealInfo.rebate) * (rela.ratio/100) * (rela.share/100);
                                crm.j_charge = dealInfo.lc_charge;
                                crm.b_money = dealInfo.rebate;
                                rebateMap.insert(dealInfo.clientId, crm);
                           }
                        }
                    }
                    if (rebateMap.contains(rela.clientId)) {
                        rebateMap[rela.clientId].avg_equity = sum_equity / days;
                    }
                }
            }
            for (int i=0; i<rebateMap.keys().size(); i++) {
                QString key = rebateMap.keys().at(i);
                AgentCrm crm = rebateMap[key];
                lcsxf_sum += crm.j_charge;
                rebate_sum += crm.re_money;
                agentCrmList.append(crm);
            }
            if (rebateMap.size() > 0) {
                AgentCrm crm;
                crm.agentName = QStringLiteral("小计：");
                crm.j_charge = lcsxf_sum;
                crm.re_money = rebate_sum;
                agentCrmList.append(crm);
            }
        }
    } else if (curRelaType == 2) {
        for (int k=0; k<infoIdList.size(); k++) {
            double lcsxf_sum = 0.000;
            double rebate_sum = 0.000;
            QList<RcmRela> rcmRelaList;
            QString rcmId = infoIdList.at(k);
            int page = db_util->getRcmRelaListPage("", rcmId);
            for (int i=1; i<=page; i++) {
                QList<RcmRela> tmpList;
                db_util->getRcmRelaList(tmpList, "", rcmId, i);
                rcmRelaList.append(tmpList);
            }
            QMap<QString, RcmCrm> rebateMap;
            for (int i=0; i<rcmRelaList.size(); i++) {
                RcmRela rela = rcmRelaList.at(i);
                if (rela.rcmId == rcmId) {
                    QList<DealInfo> dealList;
                    db_util->getDealList(dealList, "", rela.clientId, start_date, end_date);
                    for (int j=0; j<dealList.size(); j++) {
                        DealInfo dealInfo = dealList.at(j);
                        if (rela.startDate <= dealInfo.theDate && rela.endDate >= dealInfo.theDate) {
                            if (rebateMap.contains(dealInfo.clientId)) {
                                rebateMap[dealInfo.clientId].b_money = rebateMap[dealInfo.clientId].b_money + dealInfo.rebate;
                                double re_money = (dealInfo.lc_charge - dealInfo.rebate) * (rela.ratio/100) * (rela.share/100);
                                rebateMap[dealInfo.clientId].re_money = rebateMap[dealInfo.clientId].re_money + re_money;
                                rebateMap[dealInfo.clientId].j_charge = rebateMap[dealInfo.clientId].j_charge + dealInfo.lc_charge;
                            } else {
                                RcmCrm crm;
                                crm.rcmName = rela.rcmName;
                                crm.clientId = rela.clientId;
                                crm.clientName = rela.clientName;
                                crm.re_money = (dealInfo.lc_charge - dealInfo.rebate) * (rela.ratio/100) * (rela.share/100);
                                crm.j_charge = dealInfo.lc_charge;
                                crm.ratio = rela.ratio;
                                crm.b_money = dealInfo.rebate;
                                QList<ClientInfo> clInfoList;
                                db_util->getClientInfoList(clInfoList, rela.clientId, "", QDate::fromString("1999-01-01", DB_DATE_FORMATE), QDate::currentDate(), 1);
                                if (clInfoList.size() > 0) {
                                    crm.openDate = clInfoList.at(0).openDate;
                                }
                                rebateMap.insert(dealInfo.clientId, crm);
                           }
                        }
                    }
                }
            }
            for (int i=0; i<rebateMap.keys().size(); i++) {
                QString key = rebateMap.keys().at(i);
                RcmCrm crm = rebateMap[key];
                lcsxf_sum += crm.j_charge;
                rebate_sum += crm.re_money;
                rcmCrmList.append(crm);
            }
            if (rebateMap.size() > 0) {
                RcmCrm crm;
                crm.rcmName = QStringLiteral("小计：");
                crm.j_charge = lcsxf_sum;
                crm.re_money = rebate_sum;
                rcmCrmList.append(crm);
            }
        }
    } else {}
}

void CrmExportDlg::loadCrmInfo()
{
    ui->tvData->clear();
    QStringList headers;
    if (curRelaType == 0) {
        headers << QStringLiteral("居间人名称") << QStringLiteral("资金账号")
                << QStringLiteral("客户名称") << QStringLiteral("净手续费")
                << QStringLiteral("返还金额");
        ui->tvData->setColumnCount(5);
        ui->tvData->setHorizontalHeaderLabels(headers);
        ui->tvData->setRowCount(brokerCrmList.size());
        for (int i=0; i<brokerCrmList.size(); i++) {
            QTableWidgetItem *item;
            BrokerCrm crm = brokerCrmList.at(i);
            item = new QTableWidgetItem(crm.brokerName);
            ui->tvData->setItem(i, 0, item);
            item = new QTableWidgetItem(crm.clientId);
            ui->tvData->setItem(i, 1, item);
            item = new QTableWidgetItem(crm.clientName);
            ui->tvData->setItem(i, 2, item);
            item = new QTableWidgetItem(QString::number(crm.j_charge, 'f', 3));
            ui->tvData->setItem(i, 3, item);
            item = new QTableWidgetItem(QString::number(crm.re_money, 'f', 3));
            ui->tvData->setItem(i, 4, item);
        }
    } else if (curRelaType == 1) {
        headers << QStringLiteral("代理人名称") << QStringLiteral("资金账号")
                << QStringLiteral("客户名称") << QStringLiteral("日均权益")
                << QStringLiteral("净手续费") << QStringLiteral("居间人返佣")
                << QStringLiteral("返还金额");
        ui->tvData->setColumnCount(7);
        ui->tvData->setHorizontalHeaderLabels(headers);
        ui->tvData->setRowCount(agentCrmList.size());
        for (int i=0; i<agentCrmList.size(); i++) {
            QTableWidgetItem *item;
            AgentCrm crm = agentCrmList.at(i);
            item = new QTableWidgetItem(crm.agentName);
            ui->tvData->setItem(i, 0, item);
            item = new QTableWidgetItem(crm.clientId);
            ui->tvData->setItem(i, 1, item);
            item = new QTableWidgetItem(crm.clientName);
            ui->tvData->setItem(i, 2, item);
            if (crm.agentName == QStringLiteral("小计：")) {
                item = new QTableWidgetItem("");
            } else {
                item = new QTableWidgetItem(QString::number(crm.avg_equity, 'f', 3));
            }
            ui->tvData->setItem(i, 3, item);
            item = new QTableWidgetItem(QString::number(crm.j_charge, 'f', 3));
            ui->tvData->setItem(i, 4, item);
            if (crm.agentName == QStringLiteral("小计：")) {
                item = new QTableWidgetItem("");
            } else {
                item = new QTableWidgetItem(QString::number(crm.b_money, 'f', 3));
            }
            ui->tvData->setItem(i, 5, item);
            item = new QTableWidgetItem(QString::number(crm.re_money, 'f', 3));
            ui->tvData->setItem(i, 6, item);
        }
    } else if (curRelaType == 2) {
        headers << QStringLiteral("开发人名称") << QStringLiteral("资金账号")
                << QStringLiteral("客户名称") << QStringLiteral("开户日期")
                << QStringLiteral("开发比例") << QStringLiteral("净手续费")
                << QStringLiteral("居间人返佣") << QStringLiteral("返还金额");
        ui->tvData->setColumnCount(8);
        ui->tvData->setHorizontalHeaderLabels(headers);
        ui->tvData->setRowCount(rcmCrmList.size());
        for (int i=0; i<rcmCrmList.size(); i++) {
            QTableWidgetItem *item;
            RcmCrm crm = rcmCrmList.at(i);
            item = new QTableWidgetItem(crm.rcmName);
            ui->tvData->setItem(i, 0, item);
            item = new QTableWidgetItem(crm.clientId);
            ui->tvData->setItem(i, 1, item);
            item = new QTableWidgetItem(crm.clientName);
            ui->tvData->setItem(i, 2, item);
            item = new QTableWidgetItem(crm.openDate.toString(DB_DATE_FORMATE));
            ui->tvData->setItem(i, 3, item);
            if (crm.rcmName == QStringLiteral("小计：")) {
                item = new QTableWidgetItem("");
            } else {
                item = new QTableWidgetItem(QString::number(crm.ratio, 'f', 0) + "%");
            }
            ui->tvData->setItem(i, 4, item);
            item = new QTableWidgetItem(QString::number(crm.j_charge, 'f', 3));
            ui->tvData->setItem(i, 5, item);
            if (crm.rcmName == QStringLiteral("小计：")) {
                item = new QTableWidgetItem("");
            } else {
                item = new QTableWidgetItem(QString::number(crm.b_money, 'f', 3));
            }
            ui->tvData->setItem(i, 6, item);
            item = new QTableWidgetItem(QString::number(crm.re_money, 'f', 3));
            ui->tvData->setItem(i, 7, item);
        }
    } else {}
    ui->tvData->repaint();
}

void CrmExportDlg::on_btn_reset_clicked()
{
    ui->de_start->setDate(QDate::currentDate());
    ui->de_end->setDate(QDate::currentDate());
}

void CrmExportDlg::on_btn_gn_clicked()
{
    infoIdList.clear();
    QTreeWidgetItemIterator it(ui->list_rela, QTreeWidgetItemIterator::Checked);
    while (*it) {
        QString str = (*it)->text(0);
        int a = str.indexOf("(") + 1;
        int b = str.indexOf(")");
        str = str.mid(a, b-a);
        if (!str.isEmpty()) {
            infoIdList.append(str);
        }
        it++;
    }
    generateCrmInfo();
    loadCrmInfo();
    isGenerate = true;
}

void CrmExportDlg::on_btn_export_clicked()
{
    if (isGenerate) {
        path = cset.getExportPath();
        if (path.isEmpty()) {
            path = "C:\\";
            cset.setExportPath(path);
        }
        QString xlsName("");
        if (curRelaType == 0) {
            xlsName = QStringLiteral("居间人提成统计");
        } else if (curRelaType == 1) {
            xlsName = QStringLiteral("代理人提成统计");
        } else if (curRelaType == 2) {
            xlsName = QStringLiteral("开发人提成统计");
        } else {}
        QString xlsPath = QFileDialog::getSaveFileName(
            this, QStringLiteral("保存文件"), path + xlsName,
            QStringLiteral("Excel表格(*.xls)"));
        if (!xlsPath.isEmpty()) {
            QFileInfo fileInfo(xlsPath);
            cset.setExportPath(fileInfo.absoluteDir().absolutePath() + "\\");
            ExcelEngine eng(xlsPath);
            eng.Open();
            eng.SaveDataFrTable(ui->tvData);
            eng.Close();
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("导出成功"),
                QMessageBox::Yes, QMessageBox::Yes);
        } else {
            qDebug() << QStringLiteral("取消保存");
        }
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请先生成数据"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}


