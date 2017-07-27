#include "mainwindow.h"
#include <QApplication>
#include "dlglogin.h"
#include <QFile>
#include <QDebug>
#include "dbutil.h"
#include <QObject>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载QSS样式表
    QFile qss(":/mediator");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    //初始化一个数据库对象
    QSqlDatabase::addDatabase("QMYSQL");

    DbUtil db_util;
    DlgLogin login(&db_util);
    MainWindow w(&db_util);

    w.initLogin(&login);
    login.show();
    return a.exec();
}
