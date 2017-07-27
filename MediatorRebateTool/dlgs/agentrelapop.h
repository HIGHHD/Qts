#ifndef AGENTRELAPOP_H
#define AGENTRELAPOP_H

#include <QDialog>
#include "struct.h"
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

namespace Ui {
class AgentRelaPop;
}

class AgentRelaPop : public QDialog
{
    Q_OBJECT

public:
    explicit AgentRelaPop(QWidget *parent = 0);
    explicit AgentRelaPop(int save_type, AgentRela &ins, QWidget *parent = 0);
    ~AgentRelaPop();

signals:
    void relaSaved(AgentRela &ins, int save_type);

private slots:
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();

private:
    Ui::AgentRelaPop *ui;
    //记录需要更新还是增加
    int ty;
};

#endif // AGENTRELAPOP_H
