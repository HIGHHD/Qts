#include "brokerrelastatepop.h"
#include "ui_brokerrelastatepop.h"

BrokerRelaStatePop::BrokerRelaStatePop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrokerRelaStatePop)
{
    ui->setupUi(this);
}

BrokerRelaStatePop::~BrokerRelaStatePop()
{
    delete ui;
}

BrokerRelaStatePop::BrokerRelaStatePop(int save_type, BrokerRela &ins, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrokerRelaStatePop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    QRegExp dou_rx("^(-?\\d+)(\\.\\d+)?$");
    ui->le_ratio->setValidator(new QRegExpValidator(dou_rx, this));
    ui->le_start->setValidator(new QRegExpValidator(dou_rx, this));
    ui->le_end->setValidator(new QRegExpValidator(dou_rx, this));
    this->ty = save_type;

    ui->la_num->setText(QString::number(ins.sectionNum));
    ui->le_start->setText(QString::number(ins.sectionStart));
    ui->le_end->setText(QString::number(ins.sectionEnd));
    ui->le_ratio->setText(QString::number(ins.ratio));
}

void BrokerRelaStatePop::on_save_btn_clicked()
{
    BrokerRela ins;
    if (ui->le_start->text().isEmpty() || ui->le_end->text().isEmpty() || ui->le_ratio->text().isEmpty() ) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("区间相关值未填写"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
    ins.sectionNum = ui->la_num->text().toInt();
    ins.sectionStart = ui->le_start->text().toDouble();
    ins.sectionEnd = ui->le_end->text().toDouble();
    ins.ratio = ui->le_ratio->text().toDouble();
    if (ins.sectionEnd > 0 && ins.sectionStart > ins.sectionEnd) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("区间起始值不能大于区间结束值"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        emit relaSaved(ins, ty);
        close();
    }
}

void BrokerRelaStatePop::on_cancle_btn_clicked()
{
    close();
}
