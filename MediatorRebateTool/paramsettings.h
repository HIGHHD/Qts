#ifndef CPARAMSETTINGS_H
#define CPARAMSETTINGS_H

#include <QtCore>
#include <QtGui>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include "fileencrypt.h"
#include "struct.h"

class CParamSettings 
{

public:	
	QSettings *m_set;
    CEncrypt m_encrypt;
	CParamSettings(void);
    ~CParamSettings(void);

    void getUserList(QList<StUser> &userList);
    void setUserList(QList<StUser>&userList);
    int getUserIndex();
    void setUserIndex(int index);

    void getDbList(QList<StDb> &dbList);
    void setDbList(QList<StDb> &dbList);
    int getDbIndex();
    void setDbIndex(int index);

    //导入导出路径管理
    QString getImportPath();
    void setImportPath(QString path);
    QString getExportPath();
    void setExportPath(QString path);
};
#endif
