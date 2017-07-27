#include "dealexportdlg.h"
#include "ui_dealexportdlg.h"

DealExportDlg::DealExportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DealExportDlg)
{
    ui->setupUi(this);
}

DealExportDlg::DealExportDlg(DbUtil *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DealExportDlg)
{
    ui->setupUi(this);
    ui->cm_type->addItem(QStringLiteral("成交信息"));
    ui->cm_type->addItem(QStringLiteral("持仓信息"));
    ui->cm_type->addItem(QStringLiteral("资金信息"));
    ui->cm_type->setCurrentIndex(0);
    curExportType = 0;

    ui->cm_param->addItem(QStringLiteral("按资金账号查询"));
    ui->cm_param->addItem(QStringLiteral("按合约代码查询"));
    ui->cm_param->addItem(QStringLiteral("按资金账号和合约代码查询"));
    ui->cm_param->setCurrentIndex(0);

    ui->list_rela->setSortingEnabled(false);
    ui->list_rela->setColumnCount(1);

    this->db_util = db;

    connect(ui->tvParam, &QTableWidget::customContextMenuRequested, this, &DealExportDlg::show_custom_context_menu);
    on_btn_reset_clicked();
}

DealExportDlg::~DealExportDlg()
{
    delete ui;
}

void DealExportDlg::closeEvent(QCloseEvent *e)
{
    hide();
    QWidget *p = (QWidget *)parent();
    p->hide();
    e->ignore();
}

void DealExportDlg::on_cm_type_activated(int index)
{
    isGenerate = false;
    curExportType = index;
    ui->tvData->clear();
    ui->tvData->setRowCount(0);
    ui->tvData->setColumnCount(0);
    ui->tvData->repaint();
}

void DealExportDlg::show_custom_context_menu(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(this);
    QTableWidgetItem *cur = ui->tvParam->itemAt(pos);
    if (cur != NULL) {
        curParamRow = cur->row();
    } else {
        curParamRow = -1;
    }
    popMenu->addAction(QStringLiteral("添加参数"));
    if (curParamRow >= 0) {
        popMenu->addAction(QStringLiteral("删除参数"));
        popMenu->addAction(QStringLiteral("批量删除"));
    }
    connect(popMenu, &QMenu::triggered, this, &DealExportDlg::on_cus_menu);
    popMenu->exec(QCursor::pos());
}

void DealExportDlg::on_cus_menu(QAction *ac)
{
    if (ac->text() == QStringLiteral("添加参数")) {
        dealPop = new DealExportPop(curParamType);
        connect(dealPop, &DealExportPop::saveParams, this, &DealExportDlg::editParams);
        dealPop->show();
    }
    if (ac->text() == QStringLiteral("删除参数")) {
        if (curParamType == 0) {
            QString str = ui->tvParam->item(curParamRow, 0)->text();
            bool ok = params["client"].removeOne(str);
            if (ok) {
                ui->tvParam->removeRow(curParamRow);
            }
        } else if (curParamType == 1) {
            QString str = ui->tvParam->item(curParamRow, 0)->text();
            bool ok = params["code"].removeOne(str);
            if (ok) {
                ui->tvParam->removeRow(curParamRow);
            }
        } else if (curParamType == 2) {
            QString clientId = ui->tvParam->item(curParamRow, 0)->text();
            QString code = ui->tvParam->item(curParamRow, 1)->text();
            if (params.contains(clientId)) {
                bool ok = params[clientId].removeOne(code);
                if (params[clientId].size() == 0) {
                    params.remove(clientId);
                }
                if (ok) {
                    ui->tvParam->removeRow(curParamRow);
                }
            }
        } else {}
    }
    if (ac->text() == QStringLiteral("批量删除")) {
        QModelIndexList selected = ui->tvParam->selectionModel()->selectedRows();
        QList<int> del_rows;
        foreach (QModelIndex index, selected) {
            int row = index.row();
            if (curParamType == 0) {
                QString str = ui->tvParam->item(row, 0)->text();
                bool ok = params["client"].removeOne(str);
                if (ok) {
                    del_rows.append(row);
                }
            } else if (curParamType == 1) {
                QString str = ui->tvParam->item(row, 0)->text();
                bool ok = params["code"].removeOne(str);
                if (ok) {
                    del_rows.append(row);
                }
            } else if (curParamType == 2) {
                QString clientId = ui->tvParam->item(row, 0)->text();
                QString code = ui->tvParam->item(row, 1)->text();
                if (params.contains(clientId)) {
                    bool ok = params[clientId].removeOne(code);
                    if (params[clientId].size() == 0) {
                        params.remove(clientId);
                    }
                    if (ok) {
                        del_rows.append(row);
                    }
                }
            } else {}
        }
        for (int i=0; i<del_rows.size(); i++) {
            ui->tvParam->removeRow(del_rows[i]);
            for (int j=0; j<del_rows.size(); j++) {
                del_rows[j] = del_rows[j] - 1;
            }
            qDebug() << del_rows;
        }
    }
}


void DealExportDlg::on_cm_param_currentIndexChanged(int index)
{
    curParamType = index;
    params.clear();
    ui->tvParam->clear();
    ui->tvParam->setRowCount(0);
    if (curParamType == 0) {
        QStringList headers;
        headers << QStringLiteral("资金账号");
        ui->tvParam->setColumnCount(1);
        ui->tvParam->setHorizontalHeaderLabels(headers);
    } else if (curParamType == 1) {
        QStringList headers;
        headers <<QStringLiteral("合约代码");
        ui->tvParam->setColumnCount(1);
        ui->tvParam->setHorizontalHeaderLabels(headers);
    } else if (curParamType == 2) {
        QStringList headers;
        headers << QStringLiteral("资金账号") << QStringLiteral("合约代码");
        ui->tvParam->setColumnCount(2);
        ui->tvParam->setHorizontalHeaderLabels(headers);
    } else {}
    ui->tvParam->repaint();
}

void DealExportDlg::editParams(QMap<QString, QStringList> &map)
{
    ui->tvParam->clearContents();
    if (curParamType == 0) {
        for (int i=0; i<map["client"].size(); i++) {
            QString str = map["client"].at(i);
            if (!params["client"].contains(str)) {
                params["client"].append(str);
            }
        }
        ui->tvParam->setRowCount(params["client"].size());
        for (int i=0; i<params["client"].size(); i++) {
            QTableWidgetItem *item;
            item = new QTableWidgetItem(params["client"].at(i));
            ui->tvParam->setItem(i, 0, item);
        }
    } else if (curParamType == 1) {
        for (int i=0; i<map["code"].size(); i++) {
            QString str = map["code"].at(i);
            if (!params["code"].contains(str)) {
                params["code"].append(str);
            }
        }
        ui->tvParam->setRowCount(params["code"].size());
        for (int i=0; i<params["code"].size(); i++) {
            QTableWidgetItem *item;
            item = new QTableWidgetItem(params["code"].at(i));
            ui->tvParam->setItem(i, 0, item);
        }
    } else if (curParamType == 2) {
        for (int i=0; i<map.keys().size(); i++) {
            QString key = map.keys().at(i);
            if (!params.contains(key)) {
                params.insert(key, map[key]);
            } else {
                for (int j=0; j<map[key].size(); j++) {
                    QString code = map[key].at(j);
                    if (!params[key].contains(code)) {
                        params[key].append(code);
                    }
                }
            }
        }
        int rowCount = 0;
        for (int i=0; i<params.keys().size(); i++) {
            QString key = params.keys().at(i);
            rowCount += params[key].size();
        }
        ui->tvParam->setRowCount(rowCount);
        rowCount = 0;
        for (int i=0; i<params.keys().size(); i++) {
            QString key = params.keys().at(i);
            for (int j=0; j<params[key].size(); j++) {
                QString code = params[key].at(j);
                QTableWidgetItem *item;
                item = new QTableWidgetItem(key);
                ui->tvParam->setItem(rowCount, 0, item);
                item = new QTableWidgetItem(code);
                ui->tvParam->setItem(rowCount, 1, item);
                rowCount ++;
            }
        }
    } else {}
    ui->tvParam->repaint();
}

void DealExportDlg::on_list_rela_itemChanged(QTreeWidgetItem *item, int column)
{
    if(Qt::PartiallyChecked!=item->checkState(0))
        setChildCheckState(item,item->checkState(0));

    if(Qt::PartiallyChecked==item->checkState(0))
        if(!isTopItem(item))
            item->parent()->setCheckState(0,Qt::PartiallyChecked);
}

bool DealExportDlg::isTopItem(QTreeWidgetItem* item)
{
    if(!item) return false;
    if(!item->parent()) return true;
    return false;
}

void DealExportDlg::setChildCheckState(QTreeWidgetItem *item, Qt::CheckState cs)
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

void DealExportDlg::setParentCheckState(QTreeWidgetItem *item)
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

void DealExportDlg::generateDealInfo()
{
    dealInfoList.clear();
    holdInfoList.clear();
    fundInfoList.clear();
    QDate start_date = ui->de_start->date();
    QDate end_date = ui->de_end->date();

    if (curExportType == 0) {
        for (int i=0; i<infoIdList.size(); i++) {
            QString agentId = infoIdList.at(i);
            QList<AgentRela> agentRelaList;
            int page = db_util->getAgentRelaListPage("", agentId);
            for (int j=1; j<=page; j++) {
                QList<AgentRela> tmpList;
                db_util->getAgentRelaList(tmpList, "", agentId, j);
                agentRelaList.append(tmpList);
            }
            QMap<QString, QList<DealInfo>> dealInfoMap;
            for (int j=0; j<agentRelaList.size(); j++) {
                AgentRela rela = agentRelaList.at(j);
                QList<DealInfo> tmpDealList;
                db_util->getDealList(tmpDealList, "", rela.clientId, start_date, end_date);
                for (int k=0; k<tmpDealList.size(); k++) {
                    DealInfo deal = tmpDealList.at(k);
                    if (rela.startDate <= deal.theDate && rela.endDate >= deal.theDate) {
                        deal.agentName = rela.agentName;
                        QString dateStr = deal.theDate.toString(DB_DATE_FORMATE);
                        if (curParamType == 0) {
                            if (params["client"].size() > 0) {
                                if (params["client"].contains(deal.clientId)) {
                                    if (dealInfoMap.contains(dateStr)) {
                                        dealInfoMap[dateStr].append(deal);
                                    } else {
                                        QList<DealInfo> aDealList;
                                        aDealList.append(deal);
                                        dealInfoMap.insert(dateStr, aDealList);
                                    }
                                }
                            } else {
                                if (dealInfoMap.contains(dateStr)) {
                                    dealInfoMap[dateStr].append(deal);
                                } else {
                                    QList<DealInfo> aDealList;
                                    aDealList.append(deal);
                                    dealInfoMap.insert(dateStr, aDealList);
                                }
                            }
                        } else if (curParamType == 1) {
                            if (params["code"].size() > 0) {
                                if (params["code"].contains(deal.code)) {
                                    if (dealInfoMap.contains(dateStr)) {
                                        dealInfoMap[dateStr].append(deal);
                                    } else {
                                        QList<DealInfo> aDealList;
                                        aDealList.append(deal);
                                        dealInfoMap.insert(dateStr, aDealList);
                                    }
                                }
                            } else {
                                if (dealInfoMap.contains(dateStr)) {
                                    dealInfoMap[dateStr].append(deal);
                                } else {
                                    QList<DealInfo> aDealList;
                                    aDealList.append(deal);
                                    dealInfoMap.insert(dateStr, aDealList);
                                }
                            }
                        } else if (curParamType == 2) {
                            if (params.size() > 0) {
                                if (params.contains(deal.clientId)) {
                                    if (params[deal.clientId].contains(deal.code)) {
                                        if (dealInfoMap.contains(dateStr)) {
                                            dealInfoMap[dateStr].append(deal);
                                        } else {
                                            QList<DealInfo> aDealList;
                                            aDealList.append(deal);
                                            dealInfoMap.insert(dateStr, aDealList);
                                        }
                                    }
                                }
                            } else {
                                if (dealInfoMap.contains(dateStr)) {
                                    dealInfoMap[dateStr].append(deal);
                                } else {
                                    QList<DealInfo> aDealList;
                                    aDealList.append(deal);
                                    dealInfoMap.insert(dateStr, aDealList);
                                }
                            }
                        } else {}
                    }
                }
            }
            dealInfoList.append(dealInfoMap);
        }
    } else if (curExportType == 1) {
        for (int i=0; i<infoIdList.size(); i++) {
            QString agentId = infoIdList.at(i);
            QList<AgentRela> agentRelaList;
            int page = db_util->getAgentRelaListPage("", agentId);
            for (int j=1; j<=page; j++) {
                QList<AgentRela> tmpList;
                db_util->getAgentRelaList(tmpList, "", agentId, j);
                agentRelaList.append(tmpList);
            }
            QMap<QString, QList<HoldInfo>> holdInfoMap;
            for (int j=0; j<agentRelaList.size(); j++) {
                AgentRela rela = agentRelaList.at(j);
                QList<HoldInfo> tmpHoldList;
                db_util->getHoldList(tmpHoldList, rela.clientId, start_date, end_date);
                for (int k=0; k<tmpHoldList.size(); k++) {
                    HoldInfo hold = tmpHoldList.at(k);
                    if (rela.startDate <= hold.theDate && rela.endDate >= hold.theDate) {
                        hold.agentName = rela.agentName;
                        QString dateStr = hold.theDate.toString(DB_DATE_FORMATE);
                        if (curParamType == 0) {
                            if (params["client"].size() > 0) {
                                if (params["client"].contains(hold.clientId)) {
                                    if (holdInfoMap.contains(dateStr)) {
                                        holdInfoMap[dateStr].append(hold);
                                    } else {
                                        QList<HoldInfo> aHoldList;
                                        aHoldList.append(hold);
                                        holdInfoMap.insert(dateStr, aHoldList);
                                    }
                                }
                            } else {
                                if (holdInfoMap.contains(dateStr)) {
                                    holdInfoMap[dateStr].append(hold);
                                } else {
                                    QList<HoldInfo> aHoldList;
                                    aHoldList.append(hold);
                                    holdInfoMap.insert(dateStr, aHoldList);
                                }
                            }
                        } else if (curParamType == 1) {
                            if (params["code"].size() > 0) {
                                if (params["code"].contains(hold.code)) {
                                    if (holdInfoMap.contains(dateStr)) {
                                        holdInfoMap[dateStr].append(hold);
                                    } else {
                                        QList<HoldInfo> aHoldList;
                                        aHoldList.append(hold);
                                        holdInfoMap.insert(dateStr, aHoldList);
                                    }
                                }
                            } else {
                                if (holdInfoMap.contains(dateStr)) {
                                    holdInfoMap[dateStr].append(hold);
                                } else {
                                    QList<HoldInfo> aHoldList;
                                    aHoldList.append(hold);
                                    holdInfoMap.insert(dateStr, aHoldList);
                                }
                            }
                        } else if (curParamType == 2) {
                            if (params.size() > 0) {
                                if (params.contains(hold.clientId)) {
                                    if (params[hold.clientId].contains(hold.code)) {
                                        if (holdInfoMap.contains(dateStr)) {
                                            holdInfoMap[dateStr].append(hold);
                                        } else {
                                            QList<HoldInfo> aHoldList;
                                            aHoldList.append(hold);
                                            holdInfoMap.insert(dateStr, aHoldList);
                                        }
                                    }
                                }
                            } else {
                                if (holdInfoMap.contains(dateStr)) {
                                    holdInfoMap[dateStr].append(hold);
                                } else {
                                    QList<HoldInfo> aHoldList;
                                    aHoldList.append(hold);
                                    holdInfoMap.insert(dateStr, aHoldList);
                                }
                            }
                        } else {}
                    }
                }
            }
            holdInfoList.append(holdInfoMap);
        }
    } else if (curExportType == 2) {
        for (int i=0; i<infoIdList.size(); i++) {
            QString agentId = infoIdList.at(i);
            QList<AgentRela> agentRelaList;
            int page = db_util->getAgentRelaListPage("", agentId);
            for (int j=1; j<=page; j++) {
                QList<AgentRela> tmpList;
                db_util->getAgentRelaList(tmpList, "", agentId, j);
                agentRelaList.append(tmpList);
            }
            QMap<QString, QList<FundInfo>> fundInfoMap;
            for (int j=0; j<agentRelaList.size(); j++) {
                AgentRela rela = agentRelaList.at(j);
                QList<FundInfo> tmpFundList;
                db_util->getFundList(tmpFundList, rela.clientId, start_date, end_date);
                for (int k=0; k<tmpFundList.size(); k++) {
                    FundInfo fund = tmpFundList.at(k);
                    if (rela.startDate <= fund.theDate && rela.endDate >= fund.theDate) {
                        fund.agentName = rela.agentName;
                        QString dateStr = fund.theDate.toString(DB_DATE_FORMATE);
                        if (curParamType == 0) {
                            if (params["client"].size() > 0) {
                                if (params["client"].contains(fund.clientId)) {
                                    if (fundInfoMap.contains(dateStr)) {
                                        fundInfoMap[dateStr].append(fund);
                                    } else {
                                        QList<FundInfo> aFundList;
                                        aFundList.append(fund);
                                        fundInfoMap.insert(dateStr, aFundList);
                                    }
                                }
                            } else {
                                if (fundInfoMap.contains(dateStr)) {
                                    fundInfoMap[dateStr].append(fund);
                                } else {
                                    QList<FundInfo> aFundList;
                                    aFundList.append(fund);
                                    fundInfoMap.insert(dateStr, aFundList);
                                }
                            }
                        } else {
                            if (fundInfoMap.contains(dateStr)) {
                                fundInfoMap[dateStr].append(fund);
                            } else {
                                QList<FundInfo> aFundList;
                                aFundList.append(fund);
                                fundInfoMap.insert(dateStr, aFundList);
                            }
                        }
                    }
                }
            }
            fundInfoList.append(fundInfoMap);
        }
    } else {}
}

void DealExportDlg::loadDealInfo()
{
    ui->tvData->clear();
    QStringList headers;
    if (curExportType == 0) {
        headers << QStringLiteral("持仓日期") << QStringLiteral("经纪姓名")
                << QStringLiteral("资金账号") << QStringLiteral("客户简称")
                << QStringLiteral("交易所") << QStringLiteral("品种类别")
                << QStringLiteral("成交数量") << QStringLiteral("成交金额")
                << QStringLiteral("浮动平仓盈亏") << QStringLiteral("手续费")
                << QStringLiteral("交易所费用") << QStringLiteral("费用返还")
                << QStringLiteral("净手续费") << QStringLiteral("盯市平仓盈亏");
        ui->tvData->setColumnCount(14);
        ui->tvData->setHorizontalHeaderLabels(headers);
        int rowCount = 0;
        for (int i=0; i<dealInfoList.size(); i++) {
            QMap<QString, QList<DealInfo>> infoMap = dealInfoList.at(i);
            for (int j=0; j<infoMap.keys().size(); j++) {
                QString key = infoMap.keys().at(j);
                rowCount += infoMap[key].size();
            }
        }
        ui->tvData->setRowCount(rowCount);
        rowCount = 0;
        for (int i=0; i<dealInfoList.size(); i++) {
            QMap<QString, QList<DealInfo>> infoMap = dealInfoList.at(i);
            for (int j=0; j<infoMap.keys().size(); j++) {
                QString key = infoMap.keys().at(j);
                for (int k=0; k<infoMap[key].size(); k++) {
                    DealInfo info = infoMap[key].at(k);
                    QTableWidgetItem *item;
                    item = new QTableWidgetItem(info.theDate.toString(DB_DATE_FORMATE));
                    ui->tvData->setItem(rowCount, 0, item);
                    item = new QTableWidgetItem(info.agentName);
                    ui->tvData->setItem(rowCount, 1, item);
                    item = new QTableWidgetItem(info.clientId);
                    ui->tvData->setItem(rowCount, 2, item);
                    item = new QTableWidgetItem(info.clientName);
                    ui->tvData->setItem(rowCount, 3, item);
                    item = new QTableWidgetItem(info.bourse);
                    ui->tvData->setItem(rowCount, 4, item);
                    item = new QTableWidgetItem(info.code);
                    ui->tvData->setItem(rowCount, 5, item);
                    item = new QTableWidgetItem(QString::number(info.lots));
                    ui->tvData->setItem(rowCount, 6, item);
                    item = new QTableWidgetItem(QString::number(info.dealPrice, 'f', 2));
                    ui->tvData->setItem(rowCount, 7, item);
                    item = new QTableWidgetItem(QString::number(info.profit, 'f', 2));
                    ui->tvData->setItem(rowCount, 8, item);
                    item = new QTableWidgetItem(QString::number(info.charge, 'f', 2));
                    ui->tvData->setItem(rowCount, 9, item);
                    item = new QTableWidgetItem(QString::number(info.sj_charge, 'f', 2));
                    ui->tvData->setItem(rowCount, 10, item);
                    item = new QTableWidgetItem("");
                    ui->tvData->setItem(rowCount, 11, item);
                    item = new QTableWidgetItem(QString::number(info.lc_charge, 'f', 2));
                    ui->tvData->setItem(rowCount, 12, item);
                    item = new QTableWidgetItem("");
                    ui->tvData->setItem(rowCount, 13, item);
                    rowCount ++;
                }
            }
        }
    } else if (curExportType == 1) {
        headers << QStringLiteral("持仓日期") << QStringLiteral("经纪姓名")
                << QStringLiteral("资金账号") << QStringLiteral("客户简称")
                << QStringLiteral("合约代码") << QStringLiteral("买入")
                << QStringLiteral("卖出") << QStringLiteral("开仓均价")
                << QStringLiteral("结算价") << QStringLiteral("持仓盈亏")
                << QStringLiteral("履约保证金") << QStringLiteral("交易所保证金");
        ui->tvData->setColumnCount(12);
        ui->tvData->setHorizontalHeaderLabels(headers);
        int rowCount = 0;
        for (int i=0; i<holdInfoList.size(); i++) {
            QMap<QString, QList<HoldInfo>> infoMap = holdInfoList.at(i);
            for (int j=0; j<infoMap.keys().size(); j++) {
                QString key = infoMap.keys().at(j);
                rowCount += infoMap[key].size();
            }
        }
        ui->tvData->setRowCount(rowCount);
        rowCount = 0;
        for (int i=0; i<holdInfoList.size(); i++) {
            QMap<QString, QList<HoldInfo>> infoMap = holdInfoList.at(i);
            for (int j=0; j<infoMap.keys().size(); j++) {
                QString key = infoMap.keys().at(j);
                for (int k=0; k<infoMap[key].size(); k++) {
                    HoldInfo info = infoMap[key].at(k);
                    QTableWidgetItem *item;
                    item = new QTableWidgetItem(info.theDate.toString(DB_DATE_FORMATE));
                    ui->tvData->setItem(rowCount, 0, item);
                    item = new QTableWidgetItem(info.agentName);
                    ui->tvData->setItem(rowCount, 1, item);
                    item = new QTableWidgetItem(info.clientId);
                    ui->tvData->setItem(rowCount, 2, item);
                    item = new QTableWidgetItem(info.clientName);
                    ui->tvData->setItem(rowCount, 3, item);
                    item = new QTableWidgetItem(info.code);
                    ui->tvData->setItem(rowCount, 4, item);
                    item = new QTableWidgetItem(QString::number(info.longLots));
                    ui->tvData->setItem(rowCount, 5, item);
                    item = new QTableWidgetItem(QString::number(info.shortLots));
                    ui->tvData->setItem(rowCount, 6, item);
                    item = new QTableWidgetItem("");
                    ui->tvData->setItem(rowCount, 7, item);
                    item = new QTableWidgetItem(QString::number(info.settlePrice, 'f', 2));
                    ui->tvData->setItem(rowCount, 8, item);
                    item = new QTableWidgetItem(QString::number(info.profit, 'f', 2));
                    ui->tvData->setItem(rowCount, 9, item);
                    item = new QTableWidgetItem(QString::number(info.performanceBond, 'f', 2));
                    ui->tvData->setItem(rowCount, 10, item);
                    item = new QTableWidgetItem(QString::number(info.advancePaymentBond, 'f', 2));
                    ui->tvData->setItem(rowCount, 11, item);
                    rowCount ++;
                }
            }
        }
    } else if (curExportType == 2) {
        headers << QStringLiteral("日期") << QStringLiteral("经纪姓名")
                << QStringLiteral("资金账号") << QStringLiteral("客户简称")
                << QStringLiteral("上日资金") << QStringLiteral("资金存取")
                << QStringLiteral("持仓盈亏") << QStringLiteral("平仓盈亏")
                << QStringLiteral("手续费") << QStringLiteral("今日资金")
                << QStringLiteral("今日权益") << QStringLiteral("履约保证金")
                << QStringLiteral("可用余额") << QStringLiteral("风险率")
                << QStringLiteral("成交量") << QStringLiteral("持仓量");
        ui->tvData->setColumnCount(16);
        ui->tvData->setHorizontalHeaderLabels(headers);
        int rowCount = 0;
        for (int i=0; i<fundInfoList.size(); i++) {
            QMap<QString, QList<FundInfo>> infoMap = fundInfoList.at(i);
            for (int j=0; j<infoMap.keys().size(); j++) {
                QString key = infoMap.keys().at(j);
                rowCount += infoMap[key].size();
            }
        }
        ui->tvData->setRowCount(rowCount);
        rowCount = 0;
        for (int i=0; i<fundInfoList.size(); i++) {
            QMap<QString, QList<FundInfo>> infoMap = fundInfoList.at(i);
            for (int j=0; j<infoMap.keys().size(); j++) {
                QString key = infoMap.keys().at(j);
                for (int k=0; k<infoMap[key].size(); k++) {
                    FundInfo info = infoMap[key].at(k);
                    QTableWidgetItem *item;
                    item = new QTableWidgetItem(info.theDate.toString(DB_DATE_FORMATE));
                    ui->tvData->setItem(rowCount, 0, item);
                    item = new QTableWidgetItem(info.agentName);
                    ui->tvData->setItem(rowCount, 1, item);
                    item = new QTableWidgetItem(info.clientId);
                    ui->tvData->setItem(rowCount, 2, item);
                    item = new QTableWidgetItem(info.clientName);
                    ui->tvData->setItem(rowCount, 3, item);
                    item = new QTableWidgetItem(QString::number(info.pre_equity, 'f', 2));
                    ui->tvData->setItem(rowCount, 4, item);
                    item = new QTableWidgetItem(QString::number(info.accessCapital, 'f', 2));
                    ui->tvData->setItem(rowCount, 5, item);
                    item = new QTableWidgetItem(QString::number(info.FDYK, 'f', 2));
                    ui->tvData->setItem(rowCount, 6, item);
                    item = new QTableWidgetItem("");
                    ui->tvData->setItem(rowCount, 7, item);
                    item = new QTableWidgetItem(QString::number(info.charge, 'f', 2));
                    ui->tvData->setItem(rowCount, 8, item);
                    item = new QTableWidgetItem("");
                    ui->tvData->setItem(rowCount, 9, item);
                    item = new QTableWidgetItem(QString::number(info.today_equity, 'f', 2));
                    ui->tvData->setItem(rowCount, 10, item);
                    item = new QTableWidgetItem(QString::number(info.margin, 'f', 2));
                    ui->tvData->setItem(rowCount, 11, item);
                    item = new QTableWidgetItem(QString::number(info.KYZJ, 'f', 2));
                    ui->tvData->setItem(rowCount, 12, item);
                    item = new QTableWidgetItem(QString::number(info.risk, 'f', 4));
                    ui->tvData->setItem(rowCount, 13, item);
                    item = new QTableWidgetItem("");
                    ui->tvData->setItem(rowCount, 14, item);
                    item = new QTableWidgetItem("");
                    ui->tvData->setItem(rowCount, 15, item);
                    rowCount ++;
                }
            }
        }
    } else {}
    ui->tvData->repaint();
}

void DealExportDlg::on_btn_reset_clicked()
{
    on_cm_param_currentIndexChanged(0);
    on_cm_type_activated(0);
    ui->de_start->setDate(QDate::currentDate());
    ui->de_end->setDate(QDate::currentDate());
    ui->list_rela->clear();
    ui->list_rela->setHeaderLabel(QStringLiteral("代理人"));
    db_util->getDeptList(deptList, "", "");
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
    ui->list_rela->expandAll();
}

void DealExportDlg::on_btn_gn_clicked()
{
    infoIdList.clear();
    QTreeWidgetItemIterator it(ui->list_rela, QTreeWidgetItemIterator::Checked);
    while (*it) {
        QString str = (*it)->text(0);
        int a = str.indexOf("(") + 1;
        int b = str.indexOf(")");
        str = str.mid(a, b-a);
        if (a <= b) {
            infoIdList.append(str);
        }
        it++;
    }
    generateDealInfo();
    loadDealInfo();
    isGenerate = true;
}

void DealExportDlg::on_btn_export_clicked()
{
    if (isGenerate) {
        QString startStr = ui->de_start->date().toString(DB_DATE_FORMATE);
        QString endStr = ui->de_end->date().toString(DB_DATE_FORMATE);
        path = cset.getExportPath();
        if (path.isEmpty()) {
            path = "C:\\";
            cset.setExportPath(path);
        }
        QString xlsName("");
        if (curExportType == 0) {
            xlsName = QStringLiteral("每日成交查询");
            if (startStr == endStr) {
                xlsName += startStr;
            } else {
                xlsName += (startStr + "-" + endStr);
            }
        } else if (curExportType == 1) {
            xlsName = QStringLiteral("每日持仓统计");
            if (startStr == endStr) {
                xlsName += startStr;
            } else {
                xlsName += (startStr + "-" + endStr);
            }
        } else if (curExportType == 2) {
            xlsName = QStringLiteral("每日资金数据");
            if (startStr == endStr) {
                xlsName += startStr;
            } else {
                xlsName += (startStr + "-" + endStr);
            }
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

void DealExportDlg::infoChanged()
{
    on_btn_reset_clicked();
}

