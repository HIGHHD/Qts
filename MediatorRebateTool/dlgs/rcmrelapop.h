#ifndef RCMRELAPOP_H
#define RCMRELAPOP_H

#include <QDialog>
#include "struct.h"
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

namespace Ui {
class RcmRelaPop;
}

class RcmRelaPop : public QDialog
{
    Q_OBJECT

public:
    explicit RcmRelaPop(QWidget *parent = 0);
    explicit RcmRelaPop(int save_type, RcmRela &ins, QWidget *parent = 0);
    ~RcmRelaPop();

private:
    Ui::RcmRelaPop *ui;
    //记录需要更新还是增加
    int ty;

signals:
    void relaSaved(RcmRela &ins, int save_type);

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // RCMRELAPOP_H
