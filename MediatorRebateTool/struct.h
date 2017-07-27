#ifndef STRUCT_H
#define STRUCT_H

#include <QObject>
#include <QDate>

#ifndef DB_DATE_FORMATE
#define DB_DATE_FORMATE "yyyy-MM-dd"
#endif

#ifndef DEFAULT_PRECISION
#define DEFAULT_PRECISION 2
#endif

#ifndef START_DATE_STR
#define START_DATE_STR "1900-01-01"
#endif

typedef struct _ST_USER
{
    QString userId;
    QString username;
    QString password;
    int saveFlag;  //0,password not save; 1,save

    bool operator==(const _ST_USER stUser) {
        return stUser.userId == userId;
    }

} StUser;

typedef struct _INFO_USER
{
    QString id;
    QString name;
    QString password;
    int userType; //1管理权限, 0普通权限
    QString deptId;
    QString deptName;
} UserInfo;

typedef struct _ST_DATABASE
{
    QString host;
    int port;
    QString name;
    QString user;
    QString pwd;

    bool operator==(const _ST_DATABASE stDb) {
        bool bo1 = host == stDb.host;
        bool bo2 = port == stDb.port;
        bool bo3 = name == stDb.name;
        bool bo4 = user == stDb.user;
        bool bo5 = pwd == stDb.pwd;
        return bo1 && bo2 && bo3 && bo4 && bo5;
    }
    _ST_DATABASE operator=(const _ST_DATABASE stDb) {
        host = stDb.host;
        port = stDb.port;
        name = stDb.name;
        user = stDb.user;
        pwd = stDb.pwd;
        return *this;
    }
} StDb;

typedef struct _ST_DEPT
{
    QString id;
    QString name;
    QString city;
    QString address;
    QString phone;
    QString p_code;
    QString leader;

    bool operator==(const _ST_DEPT stDept) {
        return stDept.id == id;
    }
} StDept;

typedef struct _ST_Client
{
    QString clientId;
    QString clientName;
    QString rcm;
    QString agent;
    QString broker;
    int clientKind; //0个人户, 1法人户
    QDate firstDate;
    QString deptName;
    QString ibDeptId;
} StClient;

typedef struct _Info_Client
{
    QString id;
    QString name;
    int kind; //0个人户, 1法人户
    QDate openDate;
} ClientInfo;

typedef struct _Info_Broker
{
    QString id;
    QString name;
    int kind; //0个人, 1企业
    QString deptId;
    QString deptName;
} BrokerInfo;

typedef struct _Info_Agent
{
    QString id;
    QString name;
    QString deptId;
    QString deptName;
} AgentInfo;

typedef struct _Info_Recommender
{
    QString id;
    QString name;
    QString deptId;
    QString deptName;
} RcmInfo;

typedef struct _Rela_Agent
{
    QString clientId;
    QString clientName;
    QString agentId;
    QString agentName;
    double ratio;
    double share;
    QDate startDate;
    QDate endDate;
    QString ibName;
} AgentRela;

typedef struct _Rela_Broker
{
    QString clientId;
    QString clientName;
    QString brokerId;
    QString brokerName;
    double ratio;
    double sectionNum;
    double sectionStart;
    double sectionEnd;
    QDate startDate;
    QDate endDate;
} BrokerRela;

typedef struct _Rela_Rcm
{
    QString clientId;
    QString clientName;
    QString rcmId;
    QString rcmName;
    double ratio;
    double share;
    QDate startDate;
    QDate endDate;
} RcmRela;

typedef struct _Info_Hold
{
    QDate theDate;
    QString clientId;
    QString clientName;
    QString bourse;
    QString code;
    int longLots ;
    int shortLots;
    double longPrice;
    double shortPrice;
    double settlePrice;
    double profit;
    double performanceBond; //履约保证金
    double advancePaymentBond; //交易所保证金

    QString agentName;
} HoldInfo;

typedef struct _Info_Deal
{
    QDate theDate;
    QString clientId;
    QString clientName;
    QString bourse;
    QString code;
    double charge;
    double sj_charge;
    double lc_charge;
    double profit;
    double dealPrice;
    double dealSum;
    int lots;
    QString brokerId;
    QString brokerName;
    double rebate;//居间人返佣

    QString agentName;
} DealInfo;

typedef struct _Info_Fund
{
    QDate theDate;
    QString clientId;
    QString clientName;
    double risk;
    double jys_risk;
    double pre_equity;      //对应导出表中的上日资金
    double today_equity;
    double accessCapital;   //对应导出表中的资金存取
    double charge;
    double sj_charge;
    double lc_charge;
    double KYZJ;            //对应导出表中的可用余额
    double DSYK;
    double FDYK;            //对应导出表中的持仓盈亏
    double margin;          //对应导出表中的履约保证金
    double jys_margin;
    double JYK_profit;

    QString agentName;
} FundInfo;

typedef struct _Crm_Agent
{
    QString clientId;
    QString clientName;
    QString agentName;
    double j_charge;
    double re_money;
    double b_money;
    double avg_equity;
} AgentCrm;

typedef struct _Crm_Broker
{
    QString clientId;
    QString clientName;
    QString brokerName;
    double j_charge;
    double re_money;
} BrokerCrm;

typedef struct _Crm_Rcm
{
    QString clientId;
    QString clientName;
    QString rcmName;
    double ratio;
    QDate openDate;
    double j_charge;
    double re_money;
    double b_money;
} RcmCrm;

#endif // STRUCT_H

