#ifndef CLIENTINFOPOP_H
#define CLIENTINFOPOP_H

#include <QDialog>
#include "struct.h"
#include <QList>
#include <QMessageBox>

namespace Ui {
class ClientInfoPop;
}

class ClientInfoPop : public QDialog
{
    Q_OBJECT

public:
    explicit ClientInfoPop(QWidget *parent = 0);
    explicit ClientInfoPop(int save_type , ClientInfo &ins, QWidget *parent = 0);
    ~ClientInfoPop();

signals:
    void instanceSaved(ClientInfo &ins, int save_type);

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();

private:
    Ui::ClientInfoPop *ui;
    //记录需要更新还是增加
    int ty;
};

#endif // CLIENTINFOPOP_H
