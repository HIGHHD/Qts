#include "brokerinfopop.h"
#include "ui_brokerinfopop.h"

BrokerInfoPop::BrokerInfoPop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrokerInfoPop)
{
    ui->setupUi(this);
}

BrokerInfoPop::~BrokerInfoPop()
{
    delete ui;
}

BrokerInfoPop::BrokerInfoPop(int save_type, BrokerInfo &ins, QList<StDept> &deptList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrokerInfoPop)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    this->ty = save_type;

    ui->cm_kind->clear();
    ui->cm_kind->addItem(QStringLiteral("未选择"));
    ui->cm_kind->addItem(QStringLiteral("个人"));
    ui->cm_kind->addItem(QStringLiteral("企业"));

    ui->cm_dept->clear();
    ui->cm_dept->addItem(QStringLiteral("未选择"));
    int curIndex = 0;
    for (int i=0; i<deptList.size(); i++) {
        StDept dept = deptList.at(i);
        this->deptList.append(dept);
        ui->cm_dept->addItem(dept.name);
        if (ins.deptId == dept.id) {
            curIndex = i + 1;
        }
    }

    if (!ins.id.isEmpty()) {
        ui->le_id->setText(ins.id);
        ui->le_name->setText(ins.name);
        if (ins.kind>=0 && ins.kind<=1) {
            ui->cm_kind->setCurrentIndex(ins.kind + 1);
        } else {
            ui->cm_kind->setCurrentIndex(0);
        }
        ui->cm_dept->setCurrentIndex(curIndex);
    }
}

void BrokerInfoPop::on_save_btn_clicked()
{
    BrokerInfo instance;
    instance.id = ui->le_id->text();
    instance.name = ui->le_name->text();
    int a = ui->cm_kind->currentIndex();
    if (a>=1 && a<=2) {
        instance.kind = a - 1;
    } else {
        instance.kind = -1;
    }
    int index = ui->cm_dept->currentIndex();
    if (index > 0) {
        StDept dept = this->deptList.at(index-1);
        instance.deptId = dept.id;
        instance.deptName = dept.name;
    } else {
        instance.deptName = ui->cm_dept->currentText();
    }

    if (instance.id.isEmpty() || instance.name.isEmpty()) {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("居间人代码和名称不能为空"),
            QMessageBox::Yes, QMessageBox::Yes);
    } else {
        emit instanceSaved(instance, ty);
        close();
    }
}

void BrokerInfoPop::on_cancle_btn_clicked()
{
    close();
}
