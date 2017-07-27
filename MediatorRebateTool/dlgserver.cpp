#include "dlgserver.h"
#include "ui_dlgserver.h"
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

DlgServer::DlgServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgServer)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    QRegExp host_rx("^(d{1,2}|1dd|2[0-4]d|25[0-5]).(d{1,2}|1dd|2[0-4]d|25[0-5]).(d{1,2}|1dd|2[0-4]d|25[0-5]).(d{1,2}|1dd|2[0-4]d|25[0-5])$");
    QRegExp port_rx("^\\d+$");
    QRegExp other_rx("^\\w+$");
    ui->sv_host->setValidator(new QRegExpValidator(host_rx,this));
    ui->sv_port->setValidator(new QRegExpValidator(port_rx,this));
    ui->sv_user->setValidator(new QRegExpValidator(other_rx,this));
    ui->sv_db_name->setValidator(new QRegExpValidator(other_rx,this));

    this->index = -1;
    ui->sv_port->setText("3306");
}

DlgServer::DlgServer(QList<StDb>* dbList, int index, int curIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgServer)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    QRegExp host_rx("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
    QRegExp port_rx("^\\d+$");
    QRegExp other_rx("^\\w+$");
    ui->sv_host->setValidator(new QRegExpValidator(host_rx,this));
    ui->sv_port->setValidator(new QRegExpValidator(port_rx,this));
    ui->sv_user->setValidator(new QRegExpValidator(other_rx,this));
    ui->sv_db_name->setValidator(new QRegExpValidator(other_rx,this));

    db_list = dbList;
    this->index = index;
    this->curIndex = curIndex;
    if(index > 0) {
        db_info = dbList->at(index - 1);
    }
    if (!db_info.host.isEmpty()) {
        ui->sv_host->setText(db_info.host);
        ui->sv_port->setText(QString::number(db_info.port));
        ui->sv_user->setText(db_info.user);
        ui->sv_pwd->setText(db_info.pwd);
        ui->sv_db_name->setText(db_info.name);
    } else {
        ui->sv_port->setText("3306");
    }
}


DlgServer::~DlgServer()
{
    delete ui;
}


void DlgServer::on_cancel_btn_clicked()
{
    this->close();
}

bool DlgServer::isExitDbInfo(StDb& db_info)
{
    for (int i=0; i<db_list->size(); i++) {
        if (db_info == db_list->at(i)) {
            return true;
        }
    }
    return false;
}

void DlgServer::on_save_set_btn_clicked()
{
    StDb dbInfo;
    dbInfo.host = ui->sv_host->text();
    dbInfo.port = ui->sv_port->text().toInt();
    dbInfo.user = ui->sv_user->text();
    dbInfo.pwd = ui->sv_pwd->text();
    dbInfo.name = ui->sv_db_name->text();
    if (index == -1) {
        if (isExitDbInfo(dbInfo)) {
            qDebug() << "dbinfo isExit";
            this->close();
        } else {
            db_list->append(dbInfo);
            emit dbInfoSavedAndSelected(*db_list, db_list->size());
            this->close();
        }
    }
    if (index > 0) {
        if (isExitDbInfo(dbInfo)) {
            qDebug() << "dbinfo isExit";
            this->close();
        } else {
            db_list->removeAt(index-1);
            db_list->insert(index-1, dbInfo);
            emit dbInfoSavedAndSelected(*db_list, index);
            this->close();
        }
    }
}

void DlgServer::on_save_btn_clicked()
{
    StDb dbInfo;
    dbInfo.host = ui->sv_host->text();
    dbInfo.port = ui->sv_port->text().toInt();
    dbInfo.user = ui->sv_user->text();
    dbInfo.pwd = ui->sv_pwd->text();
    dbInfo.name = ui->sv_db_name->text();
    if (index == -1) {
        if (isExitDbInfo(dbInfo)) {
            qDebug() << "dbinfo isExit";
            this->close();
        } else {
            db_list->append(dbInfo);
            emit dbInfoSaved(*db_list, curIndex);
            this->close();
        }
    }
    if (index > 0) {
        if (isExitDbInfo(dbInfo)) {
            qDebug() << "dbinfo isExit";
            this->close();
        } else {
            db_list->removeAt(index-1);
            db_list->insert(index-1, dbInfo);
            emit dbInfoSaved(*db_list, curIndex);
            this->close();
        }
    }
}
