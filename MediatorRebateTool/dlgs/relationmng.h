#ifndef RELATIONMNG_H
#define RELATIONMNG_H

#include <QDialog>
#include <QTableView>
#include "dbutil.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QList>
#include "struct.h"
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QCursor>
#include <QDebug>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

#include "agentinfomng.h"
#include "brokerinfomng.h"
#include "rcminfomng.h"
#include "clientinfomng.h"
#include "agentrelapop.h"
#include "rcmrelapop.h"
#include "brokerrelapop.h"

namespace Ui {
class RelationMng;
}

class RelationMng : public QDialog
{
    Q_OBJECT

public:
    explicit RelationMng(QWidget *parent = 0);
    explicit RelationMng(DbUtil *db, AgentInfoMng *agentMng, RcmInfoMng *rcmMng, BrokerInfoMng *brokerMng, ClientInfoMng *clientMng, QWidget *parent = 0);
    void closeEvent(QCloseEvent *e);
    ~RelationMng();

public slots:
    void getClientInfo(ClientInfo &info);
    void getAgentInfo(AgentInfo &info);
    void getBrokerInfo(BrokerInfo &info);
    void getRcmInfo(RcmInfo &info);

    void delRelaAgent(AgentInfo &info);
    void updateRelaAgent(AgentInfo &info, AgentInfo &old);
    void delRelaRcm(RcmInfo &info);
    void updateRelaRcm(RcmInfo &info, RcmInfo &old);
    void delRelaBroker(BrokerInfo &info);
    void updateRelaBroker(BrokerInfo &info, BrokerInfo &old);
    void delRelaClient(ClientInfo &info);
    void updateRelaClient(ClientInfo &info, ClientInfo &old);

private slots:
    void on_cm_rela_type_currentIndexChanged(int index);
    void on_btnRela_clicked();
    void on_btnClient_clicked();
    void on_btnQuery_clicked();
    void on_btnClear_clicked();

    void on_btnGoFirst_clicked();\
    void on_btnGoPre_clicked();
    void on_btnGoNext_clicked();
    void on_btnGoLast_clicked();
    void on_btnGoNum_clicked();

    void on_cus_menu(QAction *ac);
    void show_custom_context_menu(const QPoint &pos);
    void opra_rcm_db(RcmRela &ins, int save_type);
    void opra_agent_db(AgentRela &ins, int save_type);
    void opra_broker_db(QList<BrokerRela> &list);

private:
    Ui::RelationMng *ui;
    QStandardItemModel model;
    DbUtil *db_util;
    int pageNum;
    int curPageNum;
    int curRow;
    int curRelaType;//0居间，1代理，2开发
    bool enableCusMenu;//设置是否触发右键弹窗

    AgentInfoMng *agentMng;
    RcmInfoMng *rcmMng;
    BrokerInfoMng *brokerMng;
    ClientInfoMng *clientMng;

    AgentRelaPop *agentPop;
    RcmRelaPop *rcmPop;
    BrokerRelaPop *brokerPop;

    QList<RcmRela> curRcmRelaList;
    QList<AgentRela> curAgentRelaList;
    QList<BrokerRela> curBrokerRelaList;
    ClientInfo curClientInfo;
    AgentInfo curAgentInfo;
    BrokerInfo curBrokerInfo;
    RcmInfo curRcmInfo;

    void setRcmRelaToRow(int row, RcmRela &ins);
    void setAgentRelaToRow(int row, AgentRela &ins);
    void setBrokerRelaToRow(int row, BrokerRela &ins);
    void loadInstanceTable(int relaType, QString clientId, QString relaId, int page);
};

#endif // RELATIONMNG_H
