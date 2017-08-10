#include "clientinfopop.h"
#include "ui_clientinfopop.h"

ClientInfoPop::ClientInfoPop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoPop)
{
    ui->setupUi(this);
}

ClientInfoPop::~ClientInfoPop()
{
    delete ui;
}

ClientInfoPop::ClientInfoPop(int save_type, ClientInfo &ins, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoPop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    this->ty = save_type;

    ui->cm_cl_kind->clear();
    ui->cm_cl_kind->addItem(QStringLiteral("未选择"));
    ui->cm_cl_kind->addItem(QStringLiteral("个人户"));
    ui->cm_cl_kind->addItem(QStringLiteral("法人户"));

    if (!ins.id.isEmpty()) {
        ui->le_clientid->setText(ins.id);
        ui->le_clientName->setText(ins.name);
        if (ins.kind>=0 && ins.kind<=1) {
            ui->cm_cl_kind->setCurrentIndex(ins.kind + 1);
        } else {
            ui->cm_cl_kind->setCurrentIndex(0);
        }
        ui->de_firstdate->setDate(ins.openDate);
    } else {
        ui->de_firstdate->setDate(QDate::currentDate());
    }
}

void ClientInfoPop::on_save_btn_clicked()
{
    ClientInfo instance;
    instance.id = ui->le_clientid->text();
    instance.name = ui->le_clientName->text();
    int a = ui->cm_cl_kind->currentIndex();
    if (a>=1 && a<=2) {
        instance.kind = a - 1;
    } else {
        instance.kind = -1;
    }
    instance.openDate = ui->de_firstdate->date();

    if (instance.id.isEmpty() || instance.name.isEmpty()) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("客户代码和名称不能为空"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        emit instanceSaved(instance, ty);
        close();
    }
}

void ClientInfoPop::on_cancle_btn_clicked()
{
    close();
}
