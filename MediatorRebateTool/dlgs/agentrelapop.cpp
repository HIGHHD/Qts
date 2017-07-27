#include "agentrelapop.h"
#include "ui_agentrelapop.h"

AgentRelaPop::AgentRelaPop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentRelaPop)
{
    ui->setupUi(this);
}

AgentRelaPop::~AgentRelaPop()
{
    delete ui;
}

AgentRelaPop::AgentRelaPop(int save_type, AgentRela &ins, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentRelaPop)
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
    ui->la_rela_id->setText(ins.agentId);
    ui->la_rela_name->setText(ins.agentName);
    ui->le_ratio->setText(QString::number(ins.ratio));
    ui->le_divide->setText(QString::number(ins.share));
    ui->de_start->setDate(ins.startDate);
    ui->de_end->setDate(ins.endDate);
    ui->le_ib->setText(ins.ibName);
}

void AgentRelaPop::on_save_btn_clicked()
{
    AgentRela ins;
    ins.clientId = ui->la_cl_id->text();
    ins.clientName = ui->la_cl_name->text();
    ins.agentId = ui->la_rela_id->text();
    ins.agentName = ui->la_rela_name->text();
    ins.ratio = ui->le_ratio->text().toDouble();
    ins.share = ui->le_divide->text().toDouble();
    ins.startDate = ui->de_start->date();
    ins.endDate = ui->de_end->date();
    ins.ibName = ui->le_ib->text();

    if (ins.startDate > ins.endDate) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("开始时间不能迟于结束时间"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        emit relaSaved(ins, ty);
        close();
    }
}

void AgentRelaPop::on_cancle_btn_clicked()
{
    close();
}
