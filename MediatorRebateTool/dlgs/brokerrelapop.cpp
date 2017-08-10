#include "brokerrelapop.h"
#include "ui_brokerrelapop.h"

BrokerRelaPop::BrokerRelaPop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrokerRelaPop)
{
    ui->setupUi(this);
}

BrokerRelaPop::~BrokerRelaPop()
{
    delete ui;
}

BrokerRelaPop::BrokerRelaPop(ClientInfo& client, BrokerInfo& broker, QList<BrokerRela>& brokerRelaList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrokerRelaPop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    connect(ui->tvIns, &QTableView::customContextMenuRequested, this, &BrokerRelaPop::show_custom_context_menu);

    this->client.id = client.id;
    this->client.name = client.name;
    this->broker.id = broker.id;
    this->broker.name = broker.name;

    this->insList.clear();
    for (int i=0; i<brokerRelaList.size(); i++) {
        this->insList.append(brokerRelaList.at(i));
    }
    if (this->insList.size() > 0) {
        ui->de_start->setDate(insList.at(0).startDate);
        ui->de_end->setDate(insList.at(0).endDate);
    } else {
        ui->de_start->setDate(QDate::currentDate());
        ui->de_end->setDate(QDate::currentDate());
    }

    ui->la_cl_id->setText(client.id);
    ui->la_cl_name->setText(client.name);
    ui->la_rela_id->setText(broker.id);
    ui->la_rela_name->setText(broker.name);

    model.setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("方案区间编号")));
    model.setHorizontalHeaderItem(1, new QStandardItem(QStringLiteral("区间起始值")));
    model.setHorizontalHeaderItem(2, new QStandardItem(QStringLiteral("区间结束值")));
    model.setHorizontalHeaderItem(3, new QStandardItem(QStringLiteral("提成比例")));
    model.setColumnCount(4);
    model.setRowCount(brokerRelaList.size());
    for (int i=0; i<brokerRelaList.size(); i++) {
        setInstanceToRow(i, brokerRelaList[i]);
    }
    ui->tvIns->setModel(&model);
    ui->tvIns->show();
}

void BrokerRelaPop::setInstanceToRow(int row, BrokerRela &ins)
{
    QModelIndex index;
    index = model.index(row, 0, QModelIndex());
    model.setData(index, QVariant(ins.sectionNum));
    index = model.index(row, 1, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.sectionStart, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 2, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.sectionEnd, 'f', DEFAULT_PRECISION)));
    index = model.index(row, 3, QModelIndex());
    model.setData(index, QVariant(QString::number(ins.ratio, 'f', DEFAULT_PRECISION)));
}

void BrokerRelaPop::show_custom_context_menu(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(this);
    QModelIndex cur = ui->tvIns->indexAt(pos);
    curRow = cur.row();
    if (insList.size() > 0) {
        if (curRow >= 0) {
            popMenu->addAction(QStringLiteral("添加区间"));
            popMenu->addAction(QStringLiteral("修改区间"));
            popMenu->addAction(QStringLiteral("删除区间"));
        }
    } else {
        popMenu->addAction(QStringLiteral("添加区间"));
    }
    connect(popMenu, &QMenu::triggered, this, &BrokerRelaPop::on_cus_menu);
    popMenu->exec(QCursor::pos());
}

void BrokerRelaPop::on_cus_menu(QAction *ac)
{
    int row = curRow;
    if (row >= 0) {
        curRela = insList.at(row);
    } else {
        curRela.sectionNum = 0;
        curRela.sectionStart = 0;
        curRela.sectionEnd = 0;
        curRela.ratio = 0;
    }

    if (ac->text() == QStringLiteral("添加区间")) {
        curRela.sectionNum = curRela.sectionNum + 1;
        if (curRela.sectionEnd > 0) {
            curRela.sectionStart = curRela.sectionEnd;
        }
        curRela.sectionEnd = 0;
        pop = new BrokerRelaStatePop(0, curRela);
        connect(pop, &BrokerRelaStatePop::relaSaved, this, &BrokerRelaPop::opra_list);
        pop->show();
    }
    if (ac->text() == QStringLiteral("修改区间")) {
        pop = new BrokerRelaStatePop(1, curRela);
        connect(pop, &BrokerRelaStatePop::relaSaved, this, &BrokerRelaPop::opra_list);
        pop->show();
    }
    if (ac->text() == QStringLiteral("删除区间")) {
        int result = QMessageBox::information(this, QStringLiteral("删除"), QStringLiteral("确认要删除？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result != QMessageBox::Yes) {
            return;
        }
        insList.removeAt(row);
        model.removeRow(row, QModelIndex());
    }
}

void BrokerRelaPop::opra_list(BrokerRela &instance, int save_type)
{
    int row = curRow;
    if (row < 0) {
        insList.append(instance);
        model.insertRow(0, QModelIndex());
        setInstanceToRow(0, instance);
    } else {
        //添加
        if (save_type == 0) {
            insList.insert(row+1, instance);
            model.insertRow(row+1, QModelIndex());
            setInstanceToRow(row+1, instance);
        }
        //编辑
        if (save_type == 1) {
            insList.removeAt(row);
            insList.insert(row, instance);
            setInstanceToRow(row, instance);
        }
    }
}

void BrokerRelaPop::on_save_btn_clicked()
{
    for (int i=0; i<insList.size(); i++) {
        insList[i].clientId = this->client.id;
        insList[i].clientName = this->client.name;
        insList[i].brokerId = this->broker.id;
        insList[i].brokerName = this->broker.name;
        insList[i].startDate = ui->de_start->date();
        insList[i].endDate = ui->de_end->date();
    }
    if (insList.size() >= 2) {
        bool ok = true;
        for (int i=0; i<insList.size(); i++) {
            BrokerRela rela = insList.at(i);
            if (i == insList.size()-1) {
                bool tmp = rela.sectionNum == insList.size();
                ok = ok && tmp;
                tmp = rela.sectionStart >= insList.at(i-1).sectionEnd;
                ok = ok && tmp;
            } else {
                bool tmp = rela.sectionNum == insList.at(i+1).sectionNum - 1 ;
                ok = ok && tmp;
                tmp = rela.sectionEnd <= insList.at(i+1).sectionStart;
                ok = ok && tmp;
            }
        }

        if (ok) {
            emit brokerRelasSaved(insList);
            close();
        } else {
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("区间设置有误"),
                QMessageBox::Yes, QMessageBox::Yes);
            close();
        }

    } else {
        emit brokerRelasSaved(insList);
        close();
    }
}

void BrokerRelaPop::on_cancle_btn_clicked()
{
    close();
}
