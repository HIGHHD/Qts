#include "rcmrelapop.h"
#include "ui_rcmrelapop.h"

RcmRelaPop::RcmRelaPop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RcmRelaPop)
{
    ui->setupUi(this);
}

RcmRelaPop::~RcmRelaPop()
{
    delete ui;
}

RcmRelaPop::RcmRelaPop(int save_type, RcmRela &ins, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RcmRelaPop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    QRegExp dou_rx("^(-?\\d+)(\\.\\d+)?$");
    ui->le_ratio->setValidator(new QRegExpValidator(dou_rx, this));
    ui->le_divide->setValidator(new QRegExpValidator(dou_rx, this));

    this->ty = save_type;

    ui->la_cl_id->setText(ins.clientId);
    ui->la_cl_name->setText(ins.clientName);
    ui->la_rela_id->setText(ins.rcmId);
    ui->la_rela_name->setText(ins.rcmName);
    ui->le_ratio->setText(QString::number(ins.ratio));
    ui->le_divide->setText(QString::number(ins.share));
    ui->de_start->setDate(ins.startDate);
    ui->de_end->setDate(ins.endDate);
}

void RcmRelaPop::on_save_btn_clicked()
{
    RcmRela ins;
    ins.clientId = ui->la_cl_id->text();
    ins.clientName = ui->la_cl_name->text();
    ins.rcmId = ui->la_rela_id->text();
    ins.rcmName = ui->la_rela_name->text();
    ins.ratio = ui->le_ratio->text().toDouble();
    ins.share = ui->le_divide->text().toDouble();
    ins.startDate = ui->de_start->date();
    ins.endDate = ui->de_end->date();

    if (ins.startDate > ins.endDate) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("开始时间不能迟于结束时间"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        emit relaSaved(ins, ty);
        close();
    }
}

void RcmRelaPop::on_cancle_btn_clicked()
{
    close();
}
