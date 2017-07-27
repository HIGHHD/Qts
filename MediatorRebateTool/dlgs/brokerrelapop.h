#ifndef BROKERRELAPOP_H
#define BROKERRELAPOP_H

#include <QDialog>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QList>
#include "struct.h"
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QCursor>
#include <QDebug>
#include <QMessageBox>
#include "brokerrelastatepop.h"

namespace Ui {
class BrokerRelaPop;
}

class BrokerRelaPop : public QDialog
{
    Q_OBJECT

public:
    explicit BrokerRelaPop(QWidget *parent = 0);
    explicit BrokerRelaPop(ClientInfo& client, BrokerInfo& broker, QList<BrokerRela>& brokerRelaList, QWidget *parent = 0);
    ~BrokerRelaPop();

private:
    Ui::BrokerRelaPop *ui;
    QStandardItemModel model;
    BrokerRelaStatePop *pop;
    int curRow;
    BrokerRela curRela;
    ClientInfo client;
    BrokerInfo broker;
    QList<BrokerRela> insList;

    void setInstanceToRow(int row, BrokerRela &ins);

signals:
    void brokerRelasSaved(QList<BrokerRela>& brokerRelaList);

private slots:
    void show_custom_context_menu(const QPoint &pos);
    void on_cus_menu(QAction *ac);
    void opra_list(BrokerRela &instance, int save_type);
    void on_save_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // BROKERRELAPOP_H
