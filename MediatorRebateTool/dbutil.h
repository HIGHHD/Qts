#ifndef DBUTIL_H
#define DBUTIL_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "struct.h"
#include <QList>
#include "excelengine.h"
#include <QProgressBar>

class DbUtil : public QObject
{
    Q_OBJECT
public:
    explicit DbUtil(QObject *parent = 0);
    ~DbUtil();
    //用户信息管理
    bool getUserList(QList<UserInfo> &list, QString id, QString name, QString deptId);
    bool insertUser(UserInfo &instance);
    bool updateUser(UserInfo &instance, UserInfo &old);
    bool updateUser(StDept dept, StDept old);
    bool deleteUser(UserInfo &instance);
    //部门信息管理
    bool getDeptList(QList<StDept> &list ,QString id, QString name);
    bool insertDept(StDept &dept);
    bool updateDept(StDept &dept, StDept &old);
    bool deleteDept(StDept &dept);

    //客户信息管理
    bool getClientInfoList(QList<ClientInfo> &list, QString id, QString name, QDate start, QDate end, int page);
    int getClientInfoListPage(QString id, QString name, QDate start, QDate end);
    bool insertClientInfo(ClientInfo &instance);
    bool updateClientInfo(ClientInfo &instance, ClientInfo &old);
    bool deleteClientInfo(ClientInfo &instance);

    //居间人信息管理
    bool getBrokerInfoList(QList<BrokerInfo> &list, QString id, QString name, QString deptId);
    bool insertBrokerInfo(BrokerInfo &instance);
    bool updateBrokerInfo(BrokerInfo &instance, BrokerInfo &old);
    bool updateBrokerInfo(StDept dept, StDept old);
    bool deleteBrokerInfo(BrokerInfo &instance);
    //代理人信息管理
    bool getAgentInfoList(QList<AgentInfo> &list, QString id, QString name, QString deptId);
    bool insertAgentInfo(AgentInfo &instance);
    bool updateAgentInfo(AgentInfo &instance, AgentInfo &old);
    bool updateAgentInfo(StDept dept, StDept old);
    bool deleteAgentInfo(AgentInfo &instance);
    //开发人信息管理
    bool getRcmInfoList(QList<RcmInfo> &list, QString id, QString name, QString deptId);
    bool insertRcmInfo(RcmInfo &instance);
    bool updateRcmInfo(RcmInfo &instance, RcmInfo &old);
    bool updateRcmInfo(StDept dept, StDept old);
    bool deleteRcmInfo(RcmInfo &instance);

    //居间人关系管理
    bool getBrokerRelaList(QList<BrokerRela> &list, QString clientId, QString relaId, int page);
    int getBrokerRelaListPage(QString clientId, QString relaId);
    bool insertBrokerRela(BrokerRela &rela);
    bool deleteBrokerRela(BrokerRela &rela);

    bool deleteBrokerRela(QString clientId, QString relaId);
    bool updateBrokerRela(BrokerInfo info, BrokerInfo old);
    bool updateBrokerRela(ClientInfo info, ClientInfo old);
    //代理人关系管理
    bool getAgentRelaList(QList<AgentRela> &list, QString clientId, QString relaId, int page);
    int getAgentRelaListPage(QString clientId, QString relaId);
    bool insertAgentRela(AgentRela &rela);
    bool updateAgentRela(AgentRela &rela, AgentRela &old);
    bool deleteAgentRela(AgentRela &rela);

    bool deleteAgentRela(QString clientId, QString relaId);
    bool updateAgentRela(AgentInfo info, AgentInfo old);
    bool updateAgentRela(ClientInfo info, ClientInfo old);
    //开发人关系管理
    bool getRcmRelaList(QList<RcmRela> &list, QString clientId, QString relaId, int page);
    int getRcmRelaListPage(QString clientId, QString relaId);
    bool insertRcmRela(RcmRela &rela);
    bool updateRcmRela(RcmRela &rela, RcmRela &old);
    bool deleteRcmRela(RcmRela &rela);

    bool deleteRcmRela(QString clientId, QString relaId);
    bool updateRcmRela(RcmInfo info, RcmInfo old);
    bool updateRcmRela(ClientInfo info, ClientInfo old);
    //操作excel 导入原始数据
    void fromXlsToAgentRela(QString xlsPath);
    void fromXlsToRcmRela(QString xlsPath);
    void fromXlsToBrokerRela(QString xlsPath);

    //持仓查询
    bool getHoldList(QList<HoldInfo> &list, QString clientId, QDate start, QDate end);
    bool insertHoldList(QString path, QProgressBar *bar);
    //成交查询
    bool getDealList(QList<DealInfo> &list, QString brokerId, QString clientId, QDate start, QDate end);
    bool insertDealList(QString path, QProgressBar *bar);
    //资金查询
    bool getFundList(QList<FundInfo> &list, QString clientId, QDate start, QDate end);
    bool insertFundList(QString path, QProgressBar *bar);

private:
    bool isRelase;

signals:

public slots:
    void initConn(StDb &db_info, bool& isSuccess);

};

#endif // DBUTIL_H
