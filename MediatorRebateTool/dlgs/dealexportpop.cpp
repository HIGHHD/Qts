#include "dealexportpop.h"
#include "ui_dealexportpop.h"
#include <QMessageBox>

DealExportPop::DealExportPop(int saveType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DealExportPop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ty = saveType;
    if (ty == 0) {
        ui->te_client->setEnabled(true);
        ui->te_code->setEnabled(false);
    } else if (ty == 1) {
        ui->te_client->setEnabled(false);
        ui->te_code->setEnabled(true);
    } else if (ty == 2) {
        ui->te_client->setEnabled(true);
        ui->te_code->setEnabled(true);
    } else {}
}

DealExportPop::~DealExportPop()
{
    delete ui;
}

void DealExportPop::on_btn_save_clicked()
{
    if (ty == 0) {
       QStringList list = ui->te_client->toPlainText().split(",", QString::SkipEmptyParts);
       if (list.size() > 0) {
           QMap<QString, QStringList> map;
//           map.insert("client", QStringList());
           for (int i=0; i<list.size(); i++) {
               QString str = list.at(i);
               if (!map["client"].contains(str)) {
                   map["client"].append(str);
               }
           }
           emit saveParams(map);
           close();
       }
    } else if (ty == 1) {
        QStringList list = ui->te_code->toPlainText().split(",", QString::SkipEmptyParts);
        if (list.size() > 0) {
            QMap<QString, QStringList> map;
//            map.insert("code", QStringList());
            for (int i=0; i<list.size(); i++) {
                QString str = list.at(i);
                if (!map["code"].contains(str)) {
                    map["code"].append(str);
                }
            }
            emit saveParams(map);
            close();
        }
    } else if (ty == 2) {
        QStringList clientList = ui->te_client->toPlainText().split(",", QString::SkipEmptyParts);
        QStringList codeList = ui->te_code->toPlainText().split(",", QString::SkipEmptyParts);
        if (clientList.size() > 0) {
            QStringList finalCodeList;
            for (int i=0; i<codeList.size(); i++) {
                QString code = codeList.at(i);
                if (!finalCodeList.contains(code)) {
                    finalCodeList.append(code);
                }
            }
            QMap<QString, QStringList> map;
            for (int i=0; i<clientList.size(); i++) {
                QString clientId = clientList.at(i);
                if (!map.contains(clientId)) {
                    map.insert(clientId, finalCodeList);
                }
            }
            emit saveParams(map);
            close();
        }
    } else {}
}

void DealExportPop::on_btn_cancel_clicked()
{
    close();
}
