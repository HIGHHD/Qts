#ifndef FILEENCRYPT_H
#define FILEENCRYPT_H

#include <QTextCodec>
#include <string.h>
class CEncrypt
{
public:
    CEncrypt();
    ~CEncrypt();
public:
	QString uncrypt(QString FalsePwd);
	QString encrypt(QString TruePwd);
};

#endif
