#include "dbutil.h"
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlError>

DbUtil::DbUtil(QObject *parent) :
    QObject(parent)
{
    isRelase = true;
//    isRelase = false;
}
DbUtil::~DbUtil()
{

}

void DbUtil::initConn(StDb &db_info, bool& isSuccess)
{
    QSqlDatabase active_db = QSqlDatabase::database();
    StDb db_cur;
    db_cur.host = active_db.hostName();
    db_cur.port = active_db.port();
    db_cur.user = active_db.userName();
    db_cur.pwd = active_db.password();
    db_cur.name = active_db.databaseName();
    if (db_cur == db_info && active_db.isValid()) {
        qDebug() << (QStringLiteral("该数据库已连接"));
        return;
    }
    if (active_db.isOpen()) {
        active_db.close();
    }
    active_db.setHostName(db_info.host);
    active_db.setPort(db_info.port);
    active_db.setUserName(db_info.user);
    active_db.setPassword(db_info.pwd);
    active_db.setDatabaseName(db_info.name);

    bool ok = active_db.open();
    active_db.exec("SET NAMES 'GBK'");
//    QMessageBox::information(NULL, active_db.lastError().text(), "");
    if (!ok) {
        qDebug() << (QStringLiteral("连接数据库失败！！！"));
        isSuccess = false;
    } else {
        qDebug() << (QStringLiteral("连接数据库成功！！！"));
        isSuccess = true;
    }

//    fromXlsToRcmRela(QStringLiteral("C:\\Users\\niusiyuan\\Desktop\\crm\\居间人返佣计算\\新系统\\xls\\当前开发客户关系.xls"));
//    fromXlsToAgentRela(QStringLiteral("C:\\Users\\niusiyuan\\Desktop\\crm\\居间人返佣计算\\新系统\\xls\\营业部服务客户关系.xls"));
//    fromXlsToBrokerRela(QStringLiteral("C:\\Users\\niusiyuan\\Desktop\\crm\\居间人返佣计算\\新系统\\xls\\居间人客户关系.xls"));
}

bool DbUtil::getDeptList(QList<StDept>& list ,QString id, QString name)
{
    list.clear();
    QSqlQuery query;
    QString sql("select DeptID,DeptName,City,Address,Telephone,PostCode,Leader from deptinfo where 1=1");
    if (!id.isEmpty()) {
        sql += " and DeptID=?";
    }
    if (!name.isEmpty()) {
        sql += " and DeptName like '%" + name + "%'";
    }
    sql += " order by DeptID";
    query.prepare(sql);
    if (!id.isEmpty()) {
        query.addBindValue(id);
    }
//    if (!name.isEmpty()) {
//        query.addBindValue(name);
//    }
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            StDept dept;
            dept.id = query.value(0).toString();
            dept.name = query.value(1).toString();
            dept.city = query.value(2).toString();
            dept.address = query.value(3).toString();
            dept.phone = query.value(4).toString();
            dept.p_code = query.value(5).toString();
            dept.leader = query.value(6).toString();
            list.append(dept);
        }
        qDebug() << (QStringLiteral("查询数据成功"));
    } else {
        qDebug() << (QStringLiteral("查询数据失败"));
    }
    return ok;
}

bool DbUtil::insertDept(StDept &dept)
{
    if (isRelase) {
        QList<StDept> list_now;
        getDeptList(list_now, dept.id, "");
        bool a = list_now.size() > 0;
        getDeptList(list_now, "", dept.name);
        bool b = list_now.size() > 0;
        if (a || b) {
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    QSqlQuery query;
    QString sql("insert into deptinfo(DeptID,DeptName,City,Address,Telephone,PostCode,Leader) values(?,?,?,?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, dept.id);
    query.bindValue(1, dept.name);
    query.bindValue(2, dept.city);
    query.bindValue(3, dept.address);
    query.bindValue(4, dept.phone);
    query.bindValue(5, dept.p_code);
    query.bindValue(6, dept.leader);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入数据成功"));
    } else {
        qDebug() << (QStringLiteral("插入数据失败"));
    }
    return ok;
}

bool DbUtil::updateDept(StDept &dept, StDept &old)
{
    if (isRelase) {
        QList<StDept> list_now;
        getDeptList(list_now, dept.id, "");
        bool a = list_now.size() > 0;
        getDeptList(list_now, "", dept.name);
        bool b = list_now.size() > 0;
        if (a || b) {
            if (old.id != dept.id && old.name != dept.name) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                    QMessageBox::Yes, QMessageBox::Yes);
                return false;
            }
        }
    }
    QSqlQuery query;
    QString sql("update deptinfo set DeptID=?,DeptName=?,City=?,Address=?,Telephone=?,PostCode=?,Leader=? where DeptId=?");
    query.prepare(sql);
    query.bindValue(0, dept.id);
    query.bindValue(1, dept.name);
    query.bindValue(2, dept.city);
    query.bindValue(3, dept.address);
    query.bindValue(4, dept.phone);
    query.bindValue(5, dept.p_code);
    query.bindValue(6, dept.leader);
    query.bindValue(7, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新数据成功"));
    } else {
        qDebug() << (QStringLiteral("更新数据失败"));
    }
    return ok;
}

bool DbUtil::deleteDept(StDept &dept)
{
    QSqlQuery query;
//    QString sql("delete from deptinfo where DeptId=? and DeptName=? and City=? and Address=? and Telephone=? and PostCode=? and Leader=?");
    QString sql("delete from deptinfo where DeptId=?");
    query.prepare(sql);
    query.bindValue(0, dept.id);
    qDebug() << query.boundValue(0);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除数据成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除数据失败"));
        return false;
    }
}

bool DbUtil::getUserList(QList<UserInfo> &list, QString id, QString name, QString deptId)
{
    list.clear();
    QSqlQuery query;
    QString sql("select UserID,UserName,Password,UserType,DeptID,DeptName from userinfo where 1=1");
    if (!id.isEmpty()) {
        sql += " and UserID=?";
    }
    if (!name.isEmpty()) {
        sql += " and UserName like '%" + name + "%'";
    }
    if (!deptId.isEmpty()) {
        sql += " and DeptID=?";
    }
    sql += " order by UserID";
    query.prepare(sql);
    if (!id.isEmpty()) {
        query.addBindValue(id);
    }
//    if (!name.isEmpty()) {
//        query.addBindValue(name);
//    }
    if (!deptId.isEmpty()) {
        query.addBindValue(deptId);
    }
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            UserInfo instance;
            instance.id = query.value(0).toString();
            instance.name = query.value(1).toString();
            instance.password = query.value(2).toString();
            instance.userType = query.value(3).toInt();
            instance.deptId = query.value(4).toString();
            instance.deptName = query.value(5).toString();
            list.append(instance);
        }
        qDebug() << (QStringLiteral("查询 user 成功"));
    } else {
        qDebug() << (QStringLiteral("查询 user 失败"));
    }
    return ok;
}

bool DbUtil::insertUser(UserInfo &instance)
{
    if (isRelase) {
        QList<UserInfo> list;
        getUserList(list, instance.id, "", "");
        bool a = list.size() > 0;
        getUserList(list, "", instance.name, "");
        bool b = list.size() > 0;
        if (a || b) {
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    QSqlQuery query;
    QString sql("insert into userinfo(UserID,UserName,Password,UserType,DeptID,DeptName) values(?,?,?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.password);
    query.bindValue(3, instance.userType);
    query.bindValue(4, instance.deptId);
    query.bindValue(5, instance.deptName);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入 user 成功"));
    } else {
        qDebug() << (QStringLiteral("插入 user 失败"));
    }
    return ok;
}

bool DbUtil::updateUser(UserInfo &instance, UserInfo &old)
{
    if (isRelase) {
        QList<UserInfo> list;
        getUserList(list, instance.id, "", "");
        bool a = list.size() > 0;
        getUserList(list, "", instance.name, "");
        bool b = list.size() > 0;
        if (a || b) {
            if (old.id != instance.id && old.name != instance.name) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                    QMessageBox::Yes, QMessageBox::Yes);
                return false;
            }
        }
    }
    QSqlQuery query;
    QString sql("update userinfo set UserID=?,UserName=?,Password=?,UserType=?,DeptID=?,DeptName=? where UserID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.password);
    query.bindValue(3, instance.userType);
    query.bindValue(4, instance.deptId);
    query.bindValue(5, instance.deptName);
    query.bindValue(6, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 user 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 user 失败"));
    }
    return ok;
}

bool DbUtil::updateUser(StDept dept, StDept old)
{
    QSqlQuery query;
    QString sql("update userinfo set DeptID=?,DeptName=? where DeptID=?");
    query.prepare(sql);
    query.bindValue(0, dept.id);
    query.bindValue(1, dept.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 user 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 user 失败"));
    }
    return ok;
}

bool DbUtil::deleteUser(UserInfo &instance)
{
    QSqlQuery query;
    QString sql("delete from userinfo where UserID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除 user 成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("user 没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除 user 失败"));
        return false;
    }
}

bool DbUtil::getClientInfoList(QList<ClientInfo> &list, QString id, QString name, QDate start, QDate end, int page)
{
    list.clear();
    QSqlQuery query;
    QString startStr = start.toString(DB_DATE_FORMATE);
    QString endStr = end.toString(DB_DATE_FORMATE);
    QString sql("select ClientID,ClientName,ClinetKind,OpenDate from clientsinfo where 1=1 and date(OpenDate) between '" + startStr + "' and '" + endStr + "'");
    if (!id.isEmpty()) {
        sql += " and ClientID=?";
    }
    if (!name.isEmpty()) {
        sql += " and ClientName like '%" + name + "%'";
    }
    sql += " order by ClientID limit ?, ?";
    query.prepare(sql);
    if (!id.isEmpty()) {
        query.addBindValue(id);
    }
//    if (!name.isEmpty()) {
//        query.addBindValue(name);
//    }
    query.addBindValue((page-1) * 20);
    query.addBindValue(20);
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            ClientInfo instance;
            instance.id = query.value(0).toString();
            instance.name = query.value(1).toString();
            instance.kind = query.value(2).toInt();
            instance.openDate = query.value(3).toDate();
            list.append(instance);
        }
        qDebug() << (QStringLiteral("查询client成功"));
    } else {
        qDebug() << (QStringLiteral("查询client失败"));
    }
    return ok;

}

int DbUtil::getClientInfoListPage(QString id, QString name, QDate start, QDate end)
{
    QSqlQuery query;
    int size = 0;
    QString startStr = start.toString(DB_DATE_FORMATE);
    QString endStr = end.toString(DB_DATE_FORMATE);
    QString sql("select count(*) as nums from clientsinfo where 1=1 and date(OpenDate) between '" + startStr + "' and '" + endStr + "'");
    if (!id.isEmpty()) {
        sql += (" and ClientID='" + id + "'");
    }
    if (!name.isEmpty()) {
        sql += (" and ClientName like '%" + name + "%'");
    }
    query.prepare(sql);
    bool ok = query.exec(sql);
    if (ok) {
        if (!query.next()) {
            qDebug() << (QStringLiteral("不存在"));
            return 0;
        } else {
            size = query.value(0).toInt();
            qDebug() << size << (QStringLiteral("条client记录"));
        }
    } else {
        qDebug() << (QStringLiteral("查找client记录数目失败"));
        return -1;
    }
    int page = size / 20;
    int lost = size % 20;
    if (lost > 0) {
        page = page + 1;
    }
    return page;
}

bool DbUtil::insertClientInfo(ClientInfo &instance)
{
    if (isRelase) {
        QDate start = QDate::fromString("1999-01-01", DB_DATE_FORMATE);
        QDate end = QDate::currentDate();
        int page = getClientInfoListPage(instance.id, "", start, end);
        bool a = page > 0;
        page = getClientInfoListPage("", instance.name, start, end);
        bool b = page > 0;
        if (a || b) {
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    QSqlQuery query;
    QString sql("insert into clientsinfo(ClientID,ClientName,ClinetKind,OpenDate) values(?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.kind);
    query.bindValue(3, instance.openDate);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入client成功"));
    } else {
        qDebug() << (QStringLiteral("插入client失败"));
    }
    return ok;
}

bool DbUtil::updateClientInfo(ClientInfo &instance, ClientInfo &old)
{
    if (isRelase) {
        QDate start = QDate::fromString("1999-01-01", DB_DATE_FORMATE);
        QDate end = QDate::currentDate();
        int page = getClientInfoListPage(instance.id, "", start, end);
        bool a = page > 0;
        page = getClientInfoListPage("", instance.name, start, end);
        bool b = page > 0;
        if (a || b) {
            if (old.id != instance.id && old.name != instance.name) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                    QMessageBox::Yes, QMessageBox::Yes);
                return false;
            }
        }
    }
    QSqlQuery query;
    QString sql("update clientsinfo set ClientID=?,ClientName=?,ClinetKind=?,OpenDate=? where ClientID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.kind);
    query.bindValue(3, instance.openDate);
    query.bindValue(6, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新client成功"));
    } else {
        qDebug() << (QStringLiteral("更新client失败"));
    }
    return ok;
}

bool DbUtil::deleteClientInfo(ClientInfo &instance)
{
    QSqlQuery query;
    QString sql("delete from clientsinfo where ClientID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除client成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("client没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除client失败"));
        return false;
    }
}

//居间人信息管理
bool DbUtil::getBrokerInfoList(QList<BrokerInfo> &list, QString id, QString name, QString deptId)
{
    list.clear();
    QSqlQuery query;
    QString sql("select BrokerID,BrokerName,BrokerType,DeptID,DeptName from brokersinfo where 1=1");
    if (!id.isEmpty()) {
        sql += " and BrokerID=?";
    }
    if (!name.isEmpty()) {
        sql += " and BrokerName like '%" + name + "%'";
    }
    if (!deptId.isEmpty()) {
        sql += " and DeptID=?";
    }
    sql += " order by BrokerID";
    query.prepare(sql);
    if (!id.isEmpty()) {
        query.addBindValue(id);
    }
//    if (!name.isEmpty()) {
//        query.addBindValue(name);
//    }
    if (!deptId.isEmpty()) {
        query.addBindValue(deptId);
    }
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            BrokerInfo instance;
            instance.id = query.value(0).toString();
            instance.name = query.value(1).toString();
            instance.kind = query.value(2).toInt();
            instance.deptId = query.value(3).toString();
            instance.deptName = query.value(4).toString();
            list.append(instance);
        }
        qDebug() << (QStringLiteral("查询broker成功"));
    } else {
        qDebug() << (QStringLiteral("查询broker失败"));
    }
    return ok;
}

bool DbUtil::insertBrokerInfo(BrokerInfo &instance)
{
    if (isRelase) {
        QList<BrokerInfo> list;
        getBrokerInfoList(list, instance.id, "", "");
        bool a = list.size() > 0;
        getBrokerInfoList(list, "", instance.name, "");
        bool b = list.size() > 0;
        if (a || b) {
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    QSqlQuery query;
    QString sql("insert into brokersinfo(BrokerID,BrokerName,BrokerType,DeptID,DeptName) values(?,?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.kind);
    query.bindValue(3, instance.deptId);
    query.bindValue(4, instance.deptName);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入broker成功"));
    } else {
        qDebug() << (QStringLiteral("插入broker失败"));
    }
    return ok;
}

bool DbUtil::updateBrokerInfo(BrokerInfo &instance, BrokerInfo &old)
{
    if (isRelase) {
        QList<BrokerInfo> list;
        getBrokerInfoList(list, instance.id, "", "");
        bool a = list.size() > 0;
        getBrokerInfoList(list, "", instance.name, "");
        bool b = list.size() > 0;
        if (a || b) {
            if (old.id != instance.id && old.name != instance.name) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                    QMessageBox::Yes, QMessageBox::Yes);
                return false;
            }
        }
    }
    QSqlQuery query;
    QString sql("update brokersinfo set BrokerID=?,BrokerName=?,BrokerType=?,DeptID=?,DeptName=? where BrokerID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.kind);
    query.bindValue(3, instance.deptId);
    query.bindValue(4, instance.deptName);
    query.bindValue(5, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新broker成功"));
    } else {
        qDebug() << (QStringLiteral("更新broker失败"));
    }
    return ok;
}

bool DbUtil::updateBrokerInfo(StDept dept, StDept old) {
    QSqlQuery query;
    QString sql("update brokersinfo set DeptID=?,DeptName=? where DeptID=?");
    query.prepare(sql);
    query.bindValue(0, dept.id);
    query.bindValue(1, dept.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 broker 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 broker 失败"));
    }
    return ok;
}

bool DbUtil::deleteBrokerInfo(BrokerInfo &instance)
{
    QSqlQuery query;
    QString sql("delete from brokersinfo where BrokerID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除broker成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("broker没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除broker失败"));
        return false;
    }
}

//代理人信息管理
bool DbUtil::getAgentInfoList(QList<AgentInfo> &list, QString id, QString name, QString deptId)
{
    list.clear();
    QSqlQuery query;
    QString sql("select AgentID,AgentName,DeptID,DeptName from agentsinfo where 1=1");
    if (!id.isEmpty()) {
        sql += " and AgentID=?";
    }
    if (!name.isEmpty()) {
        sql += " and AgentName like '%" + name + "%'";
    }
    if (!deptId.isEmpty()) {
        sql += " and DeptID=?";
    }
    sql += " order by AgentID";
    query.prepare(sql);
    if (!id.isEmpty()) {
        query.addBindValue(id);
    }
//    if (!name.isEmpty()) {
//        query.addBindValue(name);
//    }
    if (!deptId.isEmpty()) {
        query.addBindValue(deptId);
    }
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            AgentInfo instance;
            instance.id = query.value(0).toString();
            instance.name = query.value(1).toString();
            instance.deptId = query.value(2).toString();
            instance.deptName = query.value(3).toString();
            list.append(instance);
        }
        qDebug() << (QStringLiteral("查询agent成功"));
    } else {
        qDebug() << (QStringLiteral("查询agent失败"));
    }
    return ok;
}

bool DbUtil::insertAgentInfo(AgentInfo &instance)
{
    if (isRelase) {
        QList<AgentInfo> list;
        getAgentInfoList(list, instance.id, "", "");
        bool a = list.size() > 0;
        getAgentInfoList(list, "", instance.name, "");
        bool b = list.size() > 0;
        if (a || b) {
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    QSqlQuery query;
    QString sql("insert into agentsinfo(AgentID,AgentName,DeptID,DeptName) values(?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.deptId);
    query.bindValue(3, instance.deptName);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入agent成功"));
    } else {
        qDebug() << (QStringLiteral("插入agent失败"));
    }
    return ok;
}

bool DbUtil::updateAgentInfo(AgentInfo &instance, AgentInfo &old)
{
    if (isRelase) {
        QList<AgentInfo> list;
        getAgentInfoList(list, instance.id, "", "");
        bool a = list.size() > 0;
        getAgentInfoList(list, "", instance.name, "");
        bool b = list.size() > 0;
        if (a || b) {
            if (old.id != instance.id && old.name != instance.name) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                    QMessageBox::Yes, QMessageBox::Yes);
                return false;
            }
        }
    }
    QSqlQuery query;
    QString sql("update agentsinfo set AgentID=?,AgentName=?,DeptID=?,DeptName=? where AgentID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.deptId);
    query.bindValue(3, instance.deptName);
    query.bindValue(4, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新agent成功"));
    } else {
        qDebug() << (QStringLiteral("更新agent失败"));
    }
    return ok;
}

bool DbUtil::updateAgentInfo(StDept dept, StDept old) {
    QSqlQuery query;
    QString sql("update agentsinfo set DeptID=?,DeptName=? where DeptID=?");
    query.prepare(sql);
    query.bindValue(0, dept.id);
    query.bindValue(1, dept.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 agent 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 agent 失败"));
    }
    return ok;
}

bool DbUtil::deleteAgentInfo(AgentInfo &instance)
{
    QSqlQuery query;
    QString sql("delete from agentsinfo where AgentID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除agent成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("agent没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除agent失败"));
        return false;
    }
}

//开发人信息管理
bool DbUtil::getRcmInfoList(QList<RcmInfo> &list, QString id, QString name, QString deptId)
{
    list.clear();
    QSqlQuery query;
    QString sql("select RcmID,RcmName,DeptID,DeptName from recommondersinfo where 1=1");
    if (!id.isEmpty()) {
        sql += " and RcmID=?";
    }
    if (!name.isEmpty()) {
        sql += " and RcmName like '%" + name + "%'";
    }
    if (!deptId.isEmpty()) {
        sql += " and DeptID=?";
    }
    sql += " order by RcmID";
    query.prepare(sql);
    if (!id.isEmpty()) {
        query.addBindValue(id);
    }
//    if (!name.isEmpty()) {
//        query.addBindValue(name);
//    }
    if (!deptId.isEmpty()) {
        query.addBindValue(deptId);
    }
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            RcmInfo instance;
            instance.id = query.value(0).toString();
            instance.name = query.value(1).toString();
            instance.deptId = query.value(2).toString();
            instance.deptName = query.value(3).toString();
            list.append(instance);
        }
        qDebug() << (QStringLiteral("查询recommonder成功"));
    } else {
        qDebug() << (QStringLiteral("查询recommonder失败"));
    }
    return ok;

}

bool DbUtil::insertRcmInfo(RcmInfo &instance)
{
    if (isRelase) {
        QList<RcmInfo> list;
        getRcmInfoList(list, instance.id, "", "");
        bool a = list.size() > 0;
        getRcmInfoList(list, "", instance.name, "");
        bool b = list.size() > 0;
        if (a || b) {
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    QSqlQuery query;
    QString sql("insert into recommondersinfo(RcmID,RcmName,DeptID,DeptName) values(?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.deptId);
    query.bindValue(3, instance.deptName);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入recommonder成功"));
    } else {
        qDebug() << (QStringLiteral("插入recommonder失败"));
    }
    return ok;
}

bool DbUtil::updateRcmInfo(RcmInfo &instance, RcmInfo &old)
{
    if (isRelase) {
        QList<RcmInfo> list;
        getRcmInfoList(list, instance.id, "", "");
        bool a = list.size() > 0;
        getRcmInfoList(list, "", instance.name, "");
        bool b = list.size() > 0;
        if (a || b) {
            if (old.id != instance.id && old.name != instance.name) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("数据库中已存在相同ID或名称"),
                    QMessageBox::Yes, QMessageBox::Yes);
                return false;
            }
        }
    }
    QSqlQuery query;
    QString sql("update recommondersinfo set RcmID=?,RcmName=?,DeptID=?,DeptName=? where RcmID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    query.bindValue(1, instance.name);
    query.bindValue(2, instance.deptId);
    query.bindValue(3, instance.deptName);
    query.bindValue(4, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新recommonder成功"));
    } else {
        qDebug() << (QStringLiteral("更新recommonder失败"));
    }
    return ok;
}

bool DbUtil::updateRcmInfo(StDept dept, StDept old) {
    QSqlQuery query;
    QString sql("update recommondersinfo set DeptID=?,DeptName=? where DeptID=?");
    query.prepare(sql);
    query.bindValue(0, dept.id);
    query.bindValue(1, dept.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 recommonder 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 recommonder 失败"));
    }
    return ok;
}


bool DbUtil::deleteRcmInfo(RcmInfo &instance)
{
    QSqlQuery query;
    QString sql("delete from recommondersinfo where RcmID=?");
    query.prepare(sql);
    query.bindValue(0, instance.id);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除recommonder成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("recommonder没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除recommonder失败"));
        return false;
    }
}

//居间人关系管理
bool DbUtil::getBrokerRelaList(QList<BrokerRela> &list, QString clientId, QString relaId, int page)
{
    list.clear();
    QSqlQuery query;
    QString sql("select ClientID,ClientName,BrokerID,BrokerName,Ratio,StartDate,EndDate,SectionNum,SectionStart,SectionEnd from brokerrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    if (!relaId.isEmpty()) {
        sql += " and BrokerID=?";
    }
    sql += " order by ClientID,SectionNum limit ?, ?";
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.addBindValue(clientId);
    }
    if (!relaId.isEmpty()) {
        query.addBindValue(relaId);
    }
    query.addBindValue((page-1) * 20);
    query.addBindValue(20);
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            BrokerRela instance;
            instance.clientId = query.value(0).toString();
            instance.clientName = query.value(1).toString();
            instance.brokerId = query.value(2).toString();
            instance.brokerName = query.value(3).toString();
            instance.ratio = query.value(4).toDouble();
            instance.startDate = query.value(5).toDate();
            instance.endDate = query.value(6).toDate();
            instance.sectionNum = query.value(7).toInt();
            instance.sectionStart = query.value(8).toDouble();
            instance.sectionEnd = query.value(9).toDouble();
            list.append(instance);
        }
        qDebug() << (QStringLiteral("查询 brokerrela 成功"));
    } else {
        qDebug() << (QStringLiteral("查询 brokerrela 失败"));
    }
    return ok;
}

int DbUtil::getBrokerRelaListPage(QString clientId, QString relaId)
{
    QSqlQuery query;
    int size = 0;
    QString sql("select count(*) as nums from brokerrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += (" and ClientID='" + clientId + "'");
    }
    if (!relaId.isEmpty()) {
        sql += (" and BrokerID='" + relaId + "'");
    }
    query.prepare(sql);
    bool ok = query.exec(sql);
    if (ok) {
        if (!query.next()) {
            qDebug() << (QStringLiteral("不存在"));
            return 0;
        } else {
            size = query.value(0).toInt();
            qDebug() << size << (QStringLiteral("条 brokerrela 记录"));
        }
    } else {
        qDebug() << (QStringLiteral("查找 brokerrela 记录数目失败"));
        return -1;
    }
    int page = size / 20;
    int lost = size % 20;
    if (lost > 0) {
        page = page + 1;
    }
    return page;
}

bool DbUtil::insertBrokerRela(BrokerRela &rela)
{
    QSqlQuery query;
    QString sql("insert into brokerrelateclient(ClientID,ClientName,BrokerID,BrokerName,Ratio,StartDate,EndDate,SectionNum,SectionStart,SectionEnd) values(?,?,?,?,?,?,?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, rela.clientId);
    query.bindValue(1, rela.clientName);
    query.bindValue(2, rela.brokerId);
    query.bindValue(3, rela.brokerName);
    query.bindValue(4, rela.ratio);
    query.bindValue(5, rela.startDate);
    query.bindValue(6, rela.endDate);
    query.bindValue(7, rela.sectionNum);
    query.bindValue(8, rela.sectionStart);
    query.bindValue(9, rela.sectionEnd);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入 brokerrela 成功"));
    } else {
        qDebug() << (QStringLiteral("插入 brokerrela 失败"));
    }
    return ok;
}

bool DbUtil::deleteBrokerRela(BrokerRela &rela)
{
    QSqlQuery query;
    QString sql("delete from brokerrelateclient where ClientID=? and BrokerID=? and SectionNum=?");
    query.prepare(sql);
    query.bindValue(0, rela.clientId);
    query.bindValue(1, rela.brokerId);
    query.bindValue(2, rela.sectionNum);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除 brokerrela 成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("brokerrela 没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除 brokerrela 失败"));
        return false;
    }
}

//代理人关系管理
bool DbUtil::getAgentRelaList(QList<AgentRela> &list, QString clientId, QString relaId, int page)
{
    list.clear();
    QSqlQuery query;
    QString sql("select ClientID,ClientName,AgentID,AgentName,Ratio,Share,StartDate,EndDate,IBName from agentrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    if (!relaId.isEmpty()) {
        sql += " and AgentID=?";
    }
    sql += " order by ClientID limit ?, ?";
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.addBindValue(clientId);
    }
    if (!relaId.isEmpty()) {
        query.addBindValue(relaId);
    }
    query.addBindValue((page-1) * 20);
    query.addBindValue(20);
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            AgentRela instance;
            instance.clientId = query.value(0).toString();
            instance.clientName = query.value(1).toString();
            instance.agentId = query.value(2).toString();
            instance.agentName = query.value(3).toString();
            instance.ratio = query.value(4).toDouble();
            instance.share = query.value(5).toDouble();
            instance.startDate = query.value(6).toDate();
            instance.endDate = query.value(7).toDate();
            instance.ibName = query.value(8).toString();
            list.append(instance);
        }
        qDebug() << (QStringLiteral("查询 agentrela 成功"));
    } else {
        qDebug() << (QStringLiteral("查询 agentrela 失败"));
    }
    return ok;
}

int DbUtil::getAgentRelaListPage(QString clientId, QString relaId)
{
    QSqlQuery query;
    int size = 0;
    QString sql("select count(*) as nums from agentrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += (" and ClientID='" + clientId + "'");
    }
    if (!relaId.isEmpty()) {
        sql += (" and AgentID='" + relaId + "'");
    }
    query.prepare(sql);
    bool ok = query.exec(sql);
    if (ok) {
        if (!query.next()) {
            qDebug() << (QStringLiteral("不存在"));
            return 0;
        } else {
            size = query.value(0).toInt();
            qDebug() << size << (QStringLiteral("条 agentrela 记录"));
        }
    } else {
        qDebug() << (QStringLiteral("查找 agentrela 记录数目失败"));
        return -1;
    }
    int page = size / 20;
    int lost = size % 20;
    if (lost > 0) {
        page = page + 1;
    }
    return page;
}

bool DbUtil::insertAgentRela(AgentRela &rela)
{
    QSqlQuery query;
    QString sql("insert into agentrelateclient(ClientID,ClientName,AgentID,AgentName,Ratio,Share,StartDate,EndDate,IBName) values(?,?,?,?,?,?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, rela.clientId);
    query.bindValue(1, rela.clientName);
    query.bindValue(2, rela.agentId);
    query.bindValue(3, rela.agentName);
    query.bindValue(4, rela.ratio);
    query.bindValue(5, rela.share);
    query.bindValue(6, rela.startDate);
    query.bindValue(7, rela.endDate);
    query.bindValue(8, rela.ibName);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入 agentrela 成功"));
    } else {
        qDebug() << (QStringLiteral("插入 agentrela 失败"));
    }
    return ok;
}

bool DbUtil::updateAgentRela(AgentRela &rela, AgentRela &old)
{
    QSqlQuery query;
    QString sql("update agentrelateclient set ClientID=?,ClientName=?,AgentID=?,AgentName=?,Ratio=?,Share=?,StartDate=?,EndDate=?,IBName=? where ClientID=? and AgentID=?");
    query.prepare(sql);
    query.bindValue(0, rela.clientId);
    query.bindValue(1, rela.clientName);
    query.bindValue(2, rela.agentId);
    query.bindValue(3, rela.agentName);
    query.bindValue(4, rela.ratio);
    query.bindValue(5, rela.share);
    query.bindValue(6, rela.startDate);
    query.bindValue(7, rela.endDate);
    query.bindValue(8, rela.ibName);
    query.bindValue(9, rela.clientId);
    query.bindValue(10, old.agentId);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 agentrela 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 agentrela 失败"));
    }
    return ok;
}

bool DbUtil::deleteAgentRela(AgentRela &rela)
{
    QSqlQuery query;
    QString sql("delete from agentrelateclient where ClientID=? and AgentID=?");
    query.prepare(sql);
    query.bindValue(0, rela.clientId);
    query.bindValue(1, rela.agentId);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除 agentrela 成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("agentrela 没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除 agentrela 失败"));
        return false;
    }
}

//开发人关系管理
bool DbUtil::getRcmRelaList(QList<RcmRela> &list, QString clientId, QString relaId, int page)
{
    list.clear();
    QSqlQuery query;
    QString sql("select ClientID,ClientName,RcmID,RcmName,Ratio,Share,StartDate,EndDate from rcmrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    if (!relaId.isEmpty()) {
        sql += " and RcmID=?";
    }
    sql += " order by ClientID limit ?, ?";
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.addBindValue(clientId);
    }
    if (!relaId.isEmpty()) {
        query.addBindValue(relaId);
    }
    query.addBindValue((page-1) * 20);
    query.addBindValue(20);
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            RcmRela instance;
            instance.clientId = query.value(0).toString();
            instance.clientName = query.value(1).toString();
            instance.rcmId = query.value(2).toString();
            instance.rcmName = query.value(3).toString();
            instance.ratio = query.value(4).toDouble();
            instance.share = query.value(5).toDouble();
            instance.startDate = query.value(6).toDate();
            instance.endDate = query.value(7).toDate();
            list.append(instance);
        }
        qDebug() << (QStringLiteral("查询 rcmrela 成功"));
    } else {
        qDebug() << (QStringLiteral("查询 rcmrela 失败"));
    }
    return ok;
}

int DbUtil::getRcmRelaListPage(QString clientId, QString relaId)
{
    QSqlQuery query;
    int size = 0;
    QString sql("select count(*) as nums from rcmrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += (" and ClientID='" + clientId + "'");
    }
    if (!relaId.isEmpty()) {
        sql += (" and RcmID='" + relaId + "'");
    }
    query.prepare(sql);
    bool ok = query.exec(sql);
    if (ok) {
        if (!query.next()) {
            qDebug() << (QStringLiteral("不存在"));
            return 0;
        } else {
            size = query.value(0).toInt();
            qDebug() << size << (QStringLiteral("条 rcmrela 记录"));
        }
    } else {
        qDebug() << (QStringLiteral("查找 rcmrela 记录数目失败"));
        return -1;
    }
    int page = size / 20;
    int lost = size % 20;
    if (lost > 0) {
        page = page + 1;
    }
    return page;
}

bool DbUtil::insertRcmRela(RcmRela &rela)
{
    QSqlQuery query;
    QString sql("insert into rcmrelateclient(ClientID,ClientName,RcmID,RcmName,Ratio,Share,StartDate,EndDate) values(?,?,?,?,?,?,?,?)");
    query.prepare(sql);
    query.bindValue(0, rela.clientId);
    query.bindValue(1, rela.clientName);
    query.bindValue(2, rela.rcmId);
    query.bindValue(3, rela.rcmName);
    query.bindValue(4, rela.ratio);
    query.bindValue(5, rela.share);
    query.bindValue(6, rela.startDate);
    query.bindValue(7, rela.endDate);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("插入 rcmrela 成功"));
    } else {
        qDebug() << (QStringLiteral("插入 rcmrela 失败"));
    }
    return ok;
}

bool DbUtil::updateRcmRela(RcmRela &rela, RcmRela &old)
{
    QSqlQuery query;
    QString sql("update rcmrelateclient set ClientID=?,ClientName=?,RcmID=?,RcmName=?,Ratio=?,Share=?,StartDate=?,EndDate=? where ClientID=? and RcmID=?");
    query.prepare(sql);
    query.bindValue(0, rela.clientId);
    query.bindValue(1, rela.clientName);
    query.bindValue(2, rela.rcmId);
    query.bindValue(3, rela.rcmName);
    query.bindValue(4, rela.ratio);
    query.bindValue(5, rela.share);
    query.bindValue(6, rela.startDate);
    query.bindValue(7, rela.endDate);
    query.bindValue(8, rela.clientId);
    query.bindValue(9, old.rcmId);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 rcmrela 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 rcmrela 失败"));
    }
    return ok;
}

bool DbUtil::deleteRcmRela(RcmRela &rela)
{
    QSqlQuery query;
    QString sql("delete from rcmrelateclient where ClientID=? and RcmID=?");
    query.prepare(sql);
    query.bindValue(0, rela.clientId);
    query.bindValue(1, rela.rcmId);
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除 rcmrela 成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("rcmrela 没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除 rcmrela 失败"));
        return false;
    }
}

bool DbUtil::deleteBrokerRela(QString clientId, QString relaId)
{
    QSqlQuery query;
    QString sql("delete from brokerrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    if (!relaId.isEmpty()) {
        sql += " and BrokerID=?";
    }
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.bindValue(0, clientId);
    }
    if (!relaId.isEmpty()) {
        query.bindValue(1, relaId);
    }
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除 brokerrela 成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("brokerrela 没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除 brokerrela 失败"));
        return false;
    }
}

bool DbUtil::updateBrokerRela(BrokerInfo info, BrokerInfo old)
{
    QSqlQuery query;
    QString sql("update brokerrelateclient set BrokerID=?,BrokerName=? where BrokerID=?");
    query.prepare(sql);
    query.bindValue(0, info.id);
    query.bindValue(1, info.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 brokerrela 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 brokerrela 失败"));
    }
    return ok;
}

bool DbUtil::updateBrokerRela(ClientInfo info, ClientInfo old)
{
    QSqlQuery query;
    QString sql("update brokerrelateclient set ClientID=?,ClientName=? where ClientID=?");
    query.prepare(sql);
    query.bindValue(0, info.id);
    query.bindValue(1, info.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 brokerrela 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 brokerrela 失败"));
    }
    return ok;
}

bool DbUtil::deleteAgentRela(QString clientId, QString relaId)
{
    QSqlQuery query;
    QString sql("delete from agentrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    if (!relaId.isEmpty()) {
        sql += " and AgentID=?";
    }
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.bindValue(0, clientId);
    }
    if (!relaId.isEmpty()) {
        query.bindValue(1, relaId);
    }
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除 agentrela 成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("agentrela 没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除 agentrela 失败"));
        return false;
    }
}

bool DbUtil::updateAgentRela(AgentInfo info, AgentInfo old)
{
    QSqlQuery query;
    QString sql("update agentrelateclient set AgentID=?,AgentName=? where AgentID=?");
    query.prepare(sql);
    query.bindValue(0, info.id);
    query.bindValue(1, info.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 agentrela 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 agentrela 失败"));
    }
    return ok;
}

bool DbUtil::updateAgentRela(ClientInfo info, ClientInfo old)
{
    QSqlQuery query;
    QString sql("update agentrelateclient set ClientID=?,ClientName=? where ClientID=?");
    query.prepare(sql);
    query.bindValue(0, info.id);
    query.bindValue(1, info.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 agentrela 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 agentrela 失败"));
    }
    return ok;
}

bool DbUtil::deleteRcmRela(QString clientId, QString relaId)
{
    QSqlQuery query;
    QString sql("delete from rcmrelateclient where 1=1");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    if (!relaId.isEmpty()) {
        sql += " and RcmID=?";
    }
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.bindValue(0, clientId);
    }
    if (!relaId.isEmpty()) {
        query.bindValue(1, relaId);
    }
    bool ok = query.exec();
    if (ok) {
        int a = query.numRowsAffected();
        if (a > 0) {
            qDebug() << (QStringLiteral("删除 rcmrela 成功"));
            return true;
        } else {
            qDebug() << (QStringLiteral("rcmrela 没有受影响行"));
            return false;
        }
    } else {
        qDebug() << (QStringLiteral("删除 rcmrela 失败"));
        return false;
    }
}

bool DbUtil::updateRcmRela(RcmInfo info, RcmInfo old)
{
    QSqlQuery query;
    QString sql("update rcmrelateclient set RcmID=?,RcmName=? where RcmID=?");
    query.prepare(sql);
    query.bindValue(0, info.id);
    query.bindValue(1, info.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 rcmrela 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 rcmrela 失败"));
    }
    return ok;
}

bool DbUtil::updateRcmRela(ClientInfo info, ClientInfo old)
{
    QSqlQuery query;
    QString sql("update rcmrelateclient set ClientID=?,ClientName=? where ClientID=?");
    query.prepare(sql);
    query.bindValue(0, info.id);
    query.bindValue(1, info.name);
    query.bindValue(2, old.id);
    bool ok = query.exec();
    if (ok) {
        qDebug() << (QStringLiteral("更新 rcmrela 成功"));
    } else {
        qDebug() << (QStringLiteral("更新 rcmrela 失败"));
    }
    return ok;
}

void DbUtil::fromXlsToAgentRela(QString xlsPath)
{
    QDate start = QDate::fromString("1999-01-01", DB_DATE_FORMATE);
    QDate end = QDate::currentDate();
    ExcelEngine eng(xlsPath);
    eng.Open(xlsPath);
    UINT row_count = eng.GetRowCount();
    for (UINT i=2; i<=row_count; i++) {
        AgentRela rela;
        rela.agentId = eng.GetCellData(i, 4).toString();
        rela.agentName = eng.GetCellData(i, 4).toString();
        if (rela.agentId.isEmpty()) {
            rela.agentId = eng.GetCellData(i, 6).toString();
            rela.agentName = eng.GetCellData(i, 6).toString();
        }
        rela.clientId = eng.GetCellData(i, 2).toString();
        rela.clientName = eng.GetCellData(i, 3).toString();
        rela.ratio = eng.GetCellData(i, 8).toString().toDouble();
        rela.share = eng.GetCellData(i, 9).toString().toDouble();
        int nDays = eng.GetCellData(i, 10).toString().toInt();
        rela.startDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
        rela.startDate = rela.startDate.addDays(nDays - 2);
        nDays = eng.GetCellData(i, 11).toString().toInt();
        rela.endDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
        rela.endDate = rela.endDate.addDays(nDays -2);
        rela.ibName = eng.GetCellData(i, 15).toString();
        insertAgentRela(rela);

        QList<AgentInfo> list;
        getAgentInfoList(list, rela.agentId, "", "");
        if (list.size() == 0) {
            AgentInfo info;
            info.id = rela.agentId;
            info.name = rela.agentName;
            info.deptId = eng.GetCellData(i, 1).toString();
            info.deptName = eng.GetCellData(i, 1).toString();
            insertAgentInfo(info);
        }
        QList<StDept> deptlist;
        StDept dept;
        dept.id = eng.GetCellData(i, 1).toString();
        dept.name = eng.GetCellData(i, 1).toString();
        getDeptList(deptlist, dept.id, dept.name);
        if (deptlist.size() == 0) {
            insertDept(dept);
        }
        int page = getClientInfoListPage(rela.clientId, "", start, end);
        if (page < 1) {
            ClientInfo info;
            info.id = rela.clientId;
            info.name = rela.clientName;
            if (info.name.contains(QStringLiteral("公司"))) {
                info.kind = 1;
            } else {
                info.kind = 0;
            }
            info.openDate = QDate::fromString("2000-01-01", DB_DATE_FORMATE);
            insertClientInfo(info);
        }
    }
    eng.Close();
}

void DbUtil::fromXlsToRcmRela(QString xlsPath)
{
    QDate start = QDate::fromString("1999-01-01", DB_DATE_FORMATE);
    QDate end = QDate::currentDate();
    ExcelEngine eng(xlsPath);
    eng.Open(xlsPath);
    UINT row_count = eng.GetRowCount();
    for (UINT i=2; i<=row_count; i++) {
        RcmRela rela;
        rela.rcmId = eng.GetCellData(i, 2).toString();
        rela.rcmName = eng.GetCellData(i, 2).toString();
        rela.clientId = eng.GetCellData(i, 4).toString();
        rela.clientName = eng.GetCellData(i, 5).toString();
        rela.ratio = eng.GetCellData(i, 9).toDouble() * 100;
        rela.share = eng.GetCellData(i, 11).toDouble();
        int nDays = eng.GetCellData(i, 7).toString().toInt();
        rela.startDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
        rela.startDate = rela.startDate.addDays(nDays - 2);
        nDays = eng.GetCellData(i, 8).toString().toInt();
        rela.endDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
        rela.endDate = rela.endDate.addDays(nDays -2);
        insertRcmRela(rela);

        QList<RcmInfo> list;
        getRcmInfoList(list, rela.rcmId, "", "");
        if (list.size() == 0) {
            RcmInfo info;
            info.id = rela.rcmId;
            info.name = rela.rcmName;
            info.deptId = eng.GetCellData(i, 1).toString();
            info.deptName = eng.GetCellData(i, 1).toString();
            insertRcmInfo(info);
        }
        int page = getClientInfoListPage(rela.clientId, "", start, end);
        if (page < 1) {
            ClientInfo info;
            info.id = rela.clientId;
            info.name = rela.clientName;
            if (info.name.contains(QStringLiteral("公司"))) {
                info.kind = 1;
            } else {
                info.kind = 0;
            }
            int nDays = eng.GetCellData(i, 6).toString().toInt();
            info.openDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
            info.openDate = info.openDate.addDays(nDays - 2);
            insertClientInfo(info);
        }
        QList<StDept> deptlist;
        StDept dept;
        dept.id = eng.GetCellData(i, 1).toString();
        dept.name = eng.GetCellData(i, 1).toString();
        getDeptList(deptlist, dept.id, dept.name);
        if (deptlist.size() == 0) {
            insertDept(dept);
        }
    }
    eng.Close();
}

void DbUtil::fromXlsToBrokerRela(QString xlsPath)
{
    QDate start = QDate::fromString("1999-01-01", DB_DATE_FORMATE);
    QDate end = QDate::currentDate();
    ExcelEngine eng(xlsPath);
    eng.Open(xlsPath);
    UINT row_count = eng.GetRowCount();
    for (UINT i=2; i<=row_count; i++) {
        QString a = eng.GetCellData(i, 9).toString();
        if (a == QStringLiteral("通用阶段比例提成")) {
            BrokerRela rela;
            rela.brokerId = eng.GetCellData(i, 3).toString();
            rela.brokerName = eng.GetCellData(i,2).toString();
            rela.clientId = eng.GetCellData(i, 4).toString();
            rela.clientName = eng.GetCellData(i, 5).toString();

            int nDays = eng.GetCellData(i, 7).toString().toInt();
            rela.startDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
            rela.startDate = rela.startDate.addDays(nDays - 2);
            nDays = eng.GetCellData(i, 8).toString().toInt();
            rela.endDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
            rela.endDate = rela.endDate.addDays(nDays -2);

            for (int i=1; i<=3; i++) {
                rela.sectionNum = i;
                if (i == 1) {
                    rela.ratio = 30.00;
                    rela.sectionStart = 0.0;
                    rela.sectionEnd = 30000.0;
                } else if (i == 2) {
                    rela.ratio = 40.00;
                    rela.sectionStart = 30000.0;
                    rela.sectionEnd = 50000.0;
                } else {
                    rela.ratio = 50.00;
                    rela.sectionStart = 50000.0;
                    rela.sectionEnd = 0.0;
                }
                insertBrokerRela(rela);
            }

            QList<BrokerInfo> list;
            getBrokerInfoList(list, rela.brokerId, "", "");
            if (list.size() == 0) {
                BrokerInfo info;
                info.id = rela.brokerId;
                info.name = rela.brokerName;
                if (info.name.contains(QStringLiteral("公司"))) {
                    info.kind = 1;
                } else {
                    info.kind = 0;
                }
                info.deptId = eng.GetCellData(i, 1).toString();
                info.deptName = eng.GetCellData(i, 1).toString();
                insertBrokerInfo(info);
            }
            QList<StDept> deptlist;
            StDept dept;
            dept.id = eng.GetCellData(i, 1).toString();
            dept.name = eng.GetCellData(i, 1).toString();
            getDeptList(deptlist, dept.id, dept.name);
            if (deptlist.size() == 0) {
                insertDept(dept);
            }
            int page = getClientInfoListPage(rela.clientId, "", start, end);
            if (page < 1) {
                ClientInfo info;
                info.id = rela.clientId;
                info.name = rela.clientName;
                if (info.name.contains(QStringLiteral("公司"))) {
                    info.kind = 1;
                } else {
                    info.kind = 0;
                }
                info.openDate = QDate::fromString("2000-01-01", DB_DATE_FORMATE);
                insertClientInfo(info);
            }

        } else {
            BrokerRela rela;
            rela.brokerId = eng.GetCellData(i, 3).toString();
            rela.brokerName = eng.GetCellData(i,2).toString();
            rela.clientId = eng.GetCellData(i, 4).toString();
            rela.clientName = eng.GetCellData(i, 5).toString();
            int nDays = eng.GetCellData(i, 7).toString().toInt();
            rela.startDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
            rela.startDate = rela.startDate.addDays(nDays - 2);
            nDays = eng.GetCellData(i, 8).toString().toInt();
            rela.endDate = QDate::fromString(START_DATE_STR, DB_DATE_FORMATE);
            rela.endDate = rela.endDate.addDays(nDays -2);

            rela.sectionNum = 1;
            rela.ratio = a.mid(4,2).toDouble();
            rela.sectionStart = 0.0;
            rela.sectionEnd = 0.0;
            insertBrokerRela(rela);

            QList<BrokerInfo> list;
            getBrokerInfoList(list, rela.brokerId, "", "");
            if (list.size() == 0) {
                BrokerInfo info;
                info.id = rela.brokerId;
                info.name = rela.brokerName;
                if (info.name.contains(QStringLiteral("公司"))) {
                    info.kind = 1;
                } else {
                    info.kind = 0;
                }
                info.deptId = eng.GetCellData(i, 1).toString();
                info.deptName = eng.GetCellData(i, 1).toString();
                insertBrokerInfo(info);
            }
            QList<StDept> deptlist;
            StDept dept;
            dept.id = eng.GetCellData(i, 1).toString();
            dept.name = eng.GetCellData(i, 1).toString();
            getDeptList(deptlist, dept.id, dept.name);
            if (deptlist.size() == 0) {
                insertDept(dept);
            }
            int page = getClientInfoListPage(rela.clientId, "", start, end);
            if (page < 1) {
                ClientInfo info;
                info.id = rela.clientId;
                info.name = rela.clientName;
                if (info.name.contains(QStringLiteral("公司"))) {
                    info.kind = 1;
                } else {
                    info.kind = 0;
                }
                info.openDate = QDate::fromString("2000-01-01", DB_DATE_FORMATE);
                insertClientInfo(info);
            }
        }
    }
    eng.Close();
}

//持仓查询
bool DbUtil::getHoldList(QList<HoldInfo> &list, QString clientId, QDate start, QDate end)
{
    QString startStr = start.toString(DB_DATE_FORMATE);
    QString endStr = end.toString(DB_DATE_FORMATE);
    list.clear();
    QSqlQuery query;
    QString sql("select TheDate,ClientID,ClientName,bourse,code,LongLots,ShortLots,LongPrice,ShortPrice,SettlePrice,Profit,PerformanceBond,AdvancePaymentBond from dayholdinfo where 1=1 and date(TheDate) between '" + startStr + "' and '" + endStr + "'");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.addBindValue(clientId);
    }
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            HoldInfo info;
            info.theDate = query.value(0).toDate();
            info.clientId = query.value(1).toString();
            info.clientName = query.value(2).toString();
            info.bourse = query.value(3).toString();
            info.code = query.value(4).toString();
            info.longLots = query.value(5).toInt();
            info.shortLots = query.value(6).toInt();
            info.longPrice = query.value(7).toDouble();
            info.shortPrice = query.value(8).toDouble();
            info.settlePrice = query.value(9).toDouble();
            info.profit = query.value(10).toDouble();
            info.performanceBond = query.value(11).toDouble();
            info.advancePaymentBond = query.value(12).toDouble();
            list.append(info);
        }
        qDebug() << (QStringLiteral("查询 持仓记录 成功"));
    } else {
        qDebug() << (QStringLiteral("查询 持仓记录 失败"));
    }
    return ok;
}

bool DbUtil::insertHoldList(QString path, QProgressBar *bar)
{
    QSqlQuery query;
    bool ok = true;
    QString date_str = path.right(12).left(8);
    QDate theDate = QDate::fromString(date_str, "yyyyMMdd");

    if (!path.contains(QStringLiteral("持仓"))) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("该表不是客户持仓信息表，请确认！"),
            QMessageBox::Yes, QMessageBox::Yes);
        return false;
    }

    QString sqlDel("delete from dayholdinfo where date(TheDate) between '" + date_str + "' and '" + date_str + "'");
    query.prepare(sqlDel);
    query.exec();

    ExcelEngine eng(path);
    eng.Open(path);

    UINT row_count = eng.GetRowCount();
    UINT row_start = 0;
    for (UINT i=1; i<=row_count; i++) {
        QString a = eng.GetCellData(i, 1).toString();
        if (a==QStringLiteral("资金帐号")) {
            row_start = i + 1;
            break;
        }
    }

    bar->setRange(row_start, row_count);

    QString sql("insert into dayholdinfo(TheDate,ClientID,ClientName,bourse,code,LongLots,ShortLots,LongPrice,ShortPrice,SettlePrice,Profit,PerformanceBond,AdvancePaymentBond) values(?,?,?,?,?,?,?,?,?,?,?,?,?)");

    for (UINT i=row_start; i<=row_count; i++) {
        query.prepare(sql);
        query.bindValue(0, theDate);
        query.bindValue(1, eng.GetCellData(i, 1).toString());
        query.bindValue(2, eng.GetCellData(i, 2).toString());
        query.bindValue(3, eng.GetCellData(i, 4).toString());
        query.bindValue(4, eng.GetCellData(i, 5).toString());
        query.bindValue(5, eng.GetCellData(i, 6).toInt());
        query.bindValue(6, eng.GetCellData(i, 7).toInt());
        query.bindValue(7, eng.GetCellData(i, 8).toDouble());
        query.bindValue(8, eng.GetCellData(i, 9).toDouble());
        query.bindValue(9, eng.GetCellData(i, 10).toDouble());
        query.bindValue(10, eng.GetCellData(i, 12).toDouble());
        query.bindValue(11, eng.GetCellData(i, 11).toDouble());
        query.bindValue(12, eng.GetCellData(i, 13).toDouble());
        ok = ok && query.exec();
        if (ok) {
            bar->setValue(i);
        } else {
            sqlDel = "delete from dayholdinfo where date(TheDate) between '" + date_str + "' and '" + date_str + "'";
            query.prepare(sqlDel);
            bool del_ok = query.exec();
            if (!del_ok) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("删除已导入数据失败，请操作数据库删除今日已导入数据后重新导入！"),
                    QMessageBox::Yes, QMessageBox::Yes);
            }
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("导入失败"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    eng.Close();
    return ok;
}

//成交查询
bool DbUtil::getDealList(QList<DealInfo> &list, QString brokerId, QString clientId, QDate start, QDate end)
{
    QString startStr = start.toString(DB_DATE_FORMATE);
    QString endStr = end.toString(DB_DATE_FORMATE);
    list.clear();
    QSqlQuery query;
    QString sql("select TheDate,ClientID,ClientName,bourse,code,Charge,SJCharge,LCCharge,Profit,DealPrice,DealSum,Lots,BrokerID,BrokerName,Rebate from daydealinfo where 1=1 and date(TheDate) between '" + startStr + "' and '" + endStr + "'");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    if (!brokerId.isEmpty()) {
        sql += " and BrokerID=?";
    }
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.addBindValue(clientId);
    }
    if (!brokerId.isEmpty()) {
        query.addBindValue(brokerId);
    }
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            DealInfo info;
            info.theDate = query.value(0).toDate();
            info.clientId = query.value(1).toString();
            info.clientName = query.value(2).toString();
            info.bourse = query.value(3).toString();
            info.code = query.value(4).toString();
            info.charge = query.value(5).toDouble();
            info.sj_charge = query.value(6).toDouble();
            info.lc_charge = query.value(7).toDouble();
            info.profit = query.value(8).toDouble();
            info.dealPrice = query.value(9).toDouble();
            info.dealSum = query.value(10).toDouble();
            info.lots = query.value(11).toInt();
            info.brokerId = query.value(12).toString();
            info.brokerName = query.value(13).toString();
            info.rebate = query.value(14).toDouble();
            list.append(info);
        }
        qDebug() << (QStringLiteral("查询 成交记录 成功"));
    } else {
        qDebug() << (QStringLiteral("查询 成交记录 失败"));
    }
    return ok;
}

bool DbUtil::insertDealList(QString path, QProgressBar *bar)
{
    QSqlQuery query;
    bool ok = true;
    QString date_str = path.right(12).left(8);
    QDate theDate = QDate::fromString(date_str, "yyyyMMdd");

    if (!path.contains(QStringLiteral("成交"))) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("该表不是客户成交信息表，请确认！"),
            QMessageBox::Yes, QMessageBox::Yes);
        return false;
    }

    QString sqlDel("delete from daydealinfo where date(TheDate) between '" + date_str + "' and '" + date_str + "'");
    query.prepare(sqlDel);
    query.exec();

    ExcelEngine eng(path);
    eng.Open(path);

    UINT row_count = eng.GetRowCount();
    UINT row_start = 0;
    for (UINT i=1; i<=row_count; i++) {
        QString a = eng.GetCellData(i, 1).toString();
        if (a==QStringLiteral("资金帐号")) {
            row_start = i + 1;
            break;
        }
    }

    bar->setRange(row_start, row_count);

    QString sql("insert into daydealinfo(TheDate,ClientID,ClientName,bourse,code,Charge,SJCharge,LCCharge,Profit,DealPrice,DealSum,Lots,BrokerID,BrokerName,Rebate) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

    for (UINT i=row_start; i<=row_count; i++) {
        query.prepare(sql);
        query.bindValue(0, theDate);
        QString zjzh = eng.GetCellData(i, 1).toString();
        query.bindValue(1, zjzh);
        query.bindValue(2, eng.GetCellData(i, 2).toString());
        query.bindValue(3, eng.GetCellData(i, 5).toString());
        query.bindValue(4, eng.GetCellData(i, 6).toString());
        query.bindValue(5, eng.GetCellData(i, 9).toDouble());
        query.bindValue(6, eng.GetCellData(i, 10).toDouble());
        double lcsxf = eng.GetCellData(i, 11).toDouble();
        query.bindValue(7, lcsxf);
        query.bindValue(8, eng.GetCellData(i, 12).toDouble());
        query.bindValue(9, eng.GetCellData(i, 13).toDouble());
        query.bindValue(10, eng.GetCellData(i, 14).toDouble());
        query.bindValue(11, eng.GetCellData(i, 15).toInt());

        double re_money = 0.000;
        QList<BrokerRela> bRelaList;
        getBrokerRelaList(bRelaList, zjzh, "", 1);
        if (bRelaList.size() > 0) {
            if (bRelaList.at(0).startDate <= theDate && bRelaList.at(0).endDate >= theDate) {
                query.bindValue(12, bRelaList.at(0).brokerId);
                query.bindValue(13, bRelaList.at(0).brokerName);
                if (bRelaList.size() == 1) {
                    BrokerRela bRela = bRelaList.at(0);
                    re_money = lcsxf * (bRela.ratio/100);
                } else if (bRelaList.size() > 1) {
                    for (int i=0; i<bRelaList.size(); i++) {
                        BrokerRela tmpRela = bRelaList.at(i);
                        if (tmpRela.sectionNum < bRelaList.size()) {
                            if (lcsxf >= tmpRela.sectionStart && lcsxf < tmpRela.sectionEnd) {
                                double a = lcsxf - tmpRela.sectionStart;
                                re_money = re_money + a * (tmpRela.ratio/100);
                            } else if (lcsxf >= tmpRela.sectionEnd){
                                re_money = re_money + (tmpRela.ratio/100) * (tmpRela.sectionEnd - tmpRela.sectionStart);
                            } else {}
                        } else if (tmpRela.sectionNum == bRelaList.size()){
                            if (lcsxf > tmpRela.sectionStart) {
                                double a = lcsxf - tmpRela.sectionStart;
                                re_money = re_money + a * (tmpRela.ratio/100);
                            }
                        } else {}
                    }
                }
            }
        }
        query.bindValue(14, re_money);

        ok = ok && query.exec();
        if (ok) {
            bar->setValue(i);
        } else {
            sqlDel = "delete from daydealinfo where date(TheDate) between '" + date_str + "' and '" + date_str + "'";
            query.prepare(sqlDel);
            bool del_ok = query.exec();
            if (!del_ok) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("删除已导入数据失败，请操作数据库删除今日已导入数据后重新导入！"),
                    QMessageBox::Yes, QMessageBox::Yes);
            }
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("导入失败"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    eng.Close();
    return ok;
}

//资金查询
bool DbUtil::getFundList(QList<FundInfo> &list, QString clientId, QDate start, QDate end)
{
    QString startStr = start.toString(DB_DATE_FORMATE);
    QString endStr = end.toString(DB_DATE_FORMATE);
    list.clear();
    QSqlQuery query;
    QString sql("select TheDate,ClientID,ClientName,Risk,JYSRisk,PreEquity,TodayEquity,AccessCapital,Charge,SJCharge,LCCharge,KYZJ,DSYK,FDYK,Margin,JYSMargin,JYKProfit from dayfundinfo where 1=1 and date(TheDate) between '" + startStr + "' and '" + endStr + "'");
    if (!clientId.isEmpty()) {
        sql += " and ClientID=?";
    }
    query.prepare(sql);
    if (!clientId.isEmpty()) {
        query.addBindValue(clientId);
    }
    bool ok = query.exec();
    if (ok) {
        while (query.next()) {
            FundInfo info;
            info.theDate = query.value(0).toDate();
            info.clientId = query.value(1).toString();
            info.clientName = query.value(2).toString();
            info.risk = query.value(3).toDouble();
            info.jys_risk = query.value(4).toDouble();
            info.pre_equity = query.value(5).toDouble();
            info.today_equity = query.value(6).toDouble();
            info.accessCapital = query.value(7).toDouble();
            info.charge = query.value(8).toDouble();
            info.sj_charge = query.value(9).toDouble();
            info.lc_charge = query.value(10).toDouble();
            info.KYZJ = query.value(11).toDouble();
            info.DSYK = query.value(12).toDouble();
            info.FDYK = query.value(13).toDouble();
            info.margin = query.value(14).toDouble();
            info.jys_margin = query.value(15).toDouble();
            info.JYK_profit = query.value(16).toDouble();
            list.append(info);
        }
        qDebug() << (QStringLiteral("查询 资金记录 成功"));
    } else {
        qDebug() << (QStringLiteral("查询 资金记录 失败"));
    }
    return ok;
}

bool DbUtil::insertFundList(QString path, QProgressBar *bar)
{
    QSqlQuery query;
    bool ok = true;
    QString date_str = path.right(12).left(8);
    QDate theDate = QDate::fromString(date_str, "yyyyMMdd");

    if (!path.contains(QStringLiteral("资金"))) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("该表不是客户资金风险表，请确认！"),
            QMessageBox::Yes, QMessageBox::Yes);
        return false;
    }

    QString sqlDel("delete from dayfundinfo where date(TheDate) between '" + date_str + "' and '" + date_str + "'");
    query.prepare(sqlDel);
    query.exec();

    ExcelEngine eng(path);
    eng.Open(path);

    UINT row_count = eng.GetRowCount();
    UINT row_start = 0;
    for (UINT i=1; i<=row_count; i++) {
        QString a = eng.GetCellData(i, 1).toString();
        if (a==QStringLiteral("资金帐号")) {
            row_start = i + 1;
            break;
        }
    }

    bar->setRange(row_start, row_count);

    QString sql("insert into dayfundinfo(TheDate,ClientID,ClientName,Risk,JYSRisk,PreEquity,TodayEquity,AccessCapital,Charge,SJCharge,LCCharge,KYZJ,DSYK,FDYK,Margin,JYSMargin,JYKProfit) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

    for (UINT i=row_start; i<=row_count; i++) {
        query.prepare(sql);
        query.bindValue(0, theDate);
        query.bindValue(1, eng.GetCellData(i, 1).toString());
        query.bindValue(2, eng.GetCellData(i, 2).toString());
        query.bindValue(3, eng.GetCellData(i, 4).toDouble());
        query.bindValue(4, eng.GetCellData(i, 15).toDouble());
        query.bindValue(5, eng.GetCellData(i, 5).toDouble());
        query.bindValue(6, eng.GetCellData(i, 10).toDouble());
        query.bindValue(7, eng.GetCellData(i, 6).toDouble());
        query.bindValue(8, eng.GetCellData(i, 7).toDouble());
        query.bindValue(9, eng.GetCellData(i, 19).toDouble());
        query.bindValue(10, eng.GetCellData(i, 8).toDouble());
        query.bindValue(11, eng.GetCellData(i, 11).toDouble());
        query.bindValue(12, eng.GetCellData(i, 9).toDouble());
        query.bindValue(13, eng.GetCellData(i, 13).toDouble());
        query.bindValue(14, eng.GetCellData(i, 12).toDouble());
        query.bindValue(15, eng.GetCellData(i, 16).toDouble());
        query.bindValue(16, eng.GetCellData(i, 20).toDouble());
        ok = ok && query.exec();
        if (ok) {
            bar->setValue(i);
        } else {
            sqlDel = "delete from dayfundinfo where date(TheDate) between '" + date_str + "' and '" + date_str + "'";
            query.prepare(sqlDel);
            bool del_ok = query.exec();
            if (!del_ok) {
                QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("删除已导入数据失败，请操作数据库删除今日已导入数据后重新导入！"),
                    QMessageBox::Yes, QMessageBox::Yes);
            }
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("导入失败"),
                QMessageBox::Yes, QMessageBox::Yes);
            return false;
        }
    }
    eng.Close();
    return ok;
}
