#include "fileencrypt.h"

CEncrypt::CEncrypt(void)
{

}

CEncrypt::~CEncrypt(void)
{

}

QString  CEncrypt::uncrypt(QString FalsePwd)
{
    QString deEncryprPwd = "";
	int Len = FalsePwd.length();
	char *P1 = new char[Len+1];
	char *P3 = new char[Len+1];
    strcpy(P1, FalsePwd.toStdString().c_str());
	P1[Len] = '\0';
    int n = Len / 2;
    for(int i=0; i<Len-n; i++)
	P3[i] = P1[n+i];
    for(int i = 0; i<n; i++)
	P3[Len-n+i] = P1[i];
	P3[Len] = '\0';

	for(int i=0; i<Len; i++)
	{
        int j = P3[i] & 30;
        j = j ^ (i + Len + Len*i);
        j = j & 30;
        int k = P3[i] & 225;
        char temp = j | k;
        if(temp >= 32 && temp < 127)
		P3[i] = temp;
	}
    deEncryprPwd = P3;
	delete []P1;
	delete []P3;
    return deEncryprPwd;
}

QString CEncrypt::encrypt(QString TruePwd)
{
    QString encryprPwd = "";
	int Len  = TruePwd.length();
    char *P1 = new char[Len + 1];
    strcpy(P1, TruePwd.toStdString().c_str());
	P1[Len] = '\0';
    for (int i=0; i<Len; i++) {
        int j = P1[i] & 30;
        j = j ^ (i + Len + Len * i);
        j = j & 30;
        int k = P1[i] & 225;
        char temp = j | k;
        if (temp >= 32 && temp < 127) {
            P1[i] = temp;
        }
	}
    encryprPwd = P1;
    char *P3 = new char[Len + 1];
    int n = Len / 2;
    for (int i=0; i<n; i++) {
        P3[i] = P1[Len-n+i];
    }
    for (int i=0; i<Len-n; i++) {
        P3[n+i] = P1[i];
    }
	P3[Len] = '\0';

    encryprPwd = P3;
	delete []P1;
	delete []P3;
    return encryprPwd;
}
