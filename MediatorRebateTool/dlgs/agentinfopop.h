#ifndef AGENTINFOPOP_H
#define AGENTINFOPOP_H

#include <QDialog>
#include "struct.h"
#include <QMessageBox>
#include <QList>

namespace Ui {
class AgentInfoPop;
}

class AgentInfoPop : public QDialog
{
    Q_OBJECT

public:
    explicit AgentInfoPop(QWidget *parent = 0);
    explicit AgentInfoPop(int save_type , AgentInfo &ins, QList<StDept> &deptList, QWidget *parent = 0);
    ~AgentInfoPop();

private:
    Ui::AgentInfoPop *ui;
    //记录需要更新还是增加
    int ty;
    QList<StDept> deptList;

signals:
    void instanceSaved(AgentInfo &ins, int save_type);

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // AGENTINFOPOP_H
