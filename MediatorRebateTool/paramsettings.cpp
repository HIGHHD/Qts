#include "ParamSettings.h"


CParamSettings::CParamSettings(void)
{
    m_set = new QSettings("config/userinfo.ini", QSettings::IniFormat);
	m_set->setIniCodec("UTF-8");
}

CParamSettings::~CParamSettings(void)
{
	delete m_set;
	m_set = NULL;
}

//GetUserCount
void CParamSettings::getUserList(QList<StUser> &userList)
{
    userList.clear();
    int nSize = m_set->value("LOGIN/count").toInt();
    StUser stTmp;
    for(int i=1; i<=nSize; i++) {
        stTmp.saveFlag = m_set->value(QString("LOGIN/save%1").arg(i)).toInt();
        stTmp.userId   = m_set->value(QString("LOGIN/userid%1").arg(i)).toString();
//        QString tmp = m_set->value(QString("LOGIN/password%1").arg(i)).toString();
//        stTmp.password = m_encrypt.uncrypt(tmp);
        stTmp.password = m_set->value(QString("LOGIN/password%1").arg(i)).toString();
        stTmp.username = m_set->value(QString("LOGIN/username%1").arg(i)).toString();
        userList.append(stTmp);
    }
}

void CParamSettings::setUserList(QList<StUser> &userList)
{
    int nSize = userList.size();
    m_set->beginGroup("LOGIN");
    m_set->remove("");
    m_set->setValue("count", nSize);
    for(int i=0; i<userList.size(); i++) {
        m_set->setValue(QString("save%1").arg(i+1),userList.at(i).saveFlag);
        m_set->setValue(QString("userid%1").arg(i+1),userList.at(i).userId);
//        QString tmp = m_encrypt.encrypt(userList.at(i).password);
        m_set->setValue(QString("password%1").arg(i+1),userList.at(i).password);
//        m_set->setValue(QString("password%1").arg(i+1),tmp);
        m_set->setValue(QString("username%1").arg(i+1),userList.at(i).username);
    }
    m_set->endGroup();
}


int CParamSettings::getUserIndex(){
   int index = m_set->value("USERINDEX/index").toInt();
   return index;
}

void CParamSettings::setUserIndex(int index){
    m_set->beginGroup("USERINDEX");
    m_set->setValue("index", index);
    m_set->endGroup();
}

void CParamSettings::getDbList(QList<StDb> &dbList) {
    dbList.clear();
    int nSize = m_set->value("DATABASE/COUNT").toInt();
    StDb stTmp;
    for(int i=1; i<=nSize; i++) {
        stTmp.host = m_set->value(QString("DATABASE/host%1").arg(i)).toString();
        stTmp.port = m_set->value(QString("DATABASE/port%1").arg(i)).toInt();
        stTmp.user = m_set->value(QString("DATABASE/user%1").arg(i)).toString();
        QString str = m_set->value(QString("DATABASE/pwd%1").arg(i)).toString();
        stTmp.pwd = m_encrypt.uncrypt(str);
//        stTmp.pwd  = str;
        stTmp.name = m_set->value(QString("DATABASE/name%1").arg(i)).toString();
        dbList.append(stTmp);
    }
}

void CParamSettings::setDbList(QList<StDb> &dbList)
{
    int nSize = dbList.size();
    m_set->beginGroup("DATABASE");
    m_set->remove("");
    m_set->setValue("COUNT", nSize);
    for(int i=0; i<nSize; i++) {
        m_set->setValue(QString("host%1").arg(i+1), dbList.at(i).host);
        m_set->setValue(QString("port%1").arg(i+1), dbList.at(i).port);
        m_set->setValue(QString("user%1").arg(i+1), dbList.at(i).user);
        QString str = m_encrypt.encrypt(dbList.at(i).pwd);
        m_set->setValue(QString("pwd%1").arg(i+1), str);
//        m_set->setValue(QString("pwd%1").arg(i+1), dbList.at(i).pwd);
        m_set->setValue(QString("name%1").arg(i+1), dbList.at(i).name);
    }
    m_set->endGroup();
}

int CParamSettings::getDbIndex(){
   int index = m_set->value("DBINDEX/index").toInt();
   return index;
}

void CParamSettings::setDbIndex(int index){
    m_set->beginGroup("DBINDEX");
    m_set->setValue("index", index);
    m_set->endGroup();
}

QString CParamSettings::getImportPath()
{
    QString path = m_set->value("IMPORTPATH/path").toString();
    return path;
}

void CParamSettings::setImportPath(QString path)
{
    m_set->beginGroup("IMPORTPATH");
    m_set->setValue("path", path);
    m_set->endGroup();
}

QString CParamSettings::getExportPath()
{
    QString path = m_set->value("EXPORTPATH/path").toString();
    return path;
}

void CParamSettings::setExportPath(QString path)
{
    m_set->beginGroup("EXPORTPATH");
    m_set->setValue("path", path);
    m_set->endGroup();
}
