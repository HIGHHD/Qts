#ifndef BROKERRELASTATEPOP_H
#define BROKERRELASTATEPOP_H

#include <QDialog>
#include "struct.h"
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

namespace Ui {
class BrokerRelaStatePop;
}

class BrokerRelaStatePop : public QDialog
{
    Q_OBJECT

public:
    explicit BrokerRelaStatePop(QWidget *parent = 0);
    explicit BrokerRelaStatePop(int save_type, BrokerRela &ins, QWidget *parent = 0);
    ~BrokerRelaStatePop();

signals:
    void relaSaved(BrokerRela &ins, int save_type);

private:
    Ui::BrokerRelaStatePop *ui;
    //记录需要更新还是增加
    int ty;

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // BROKERRELASTATEPOP_H
