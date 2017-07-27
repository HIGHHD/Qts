#include "dayimportdlg.h"
#include "ui_dayimportdlg.h"

DayImportDlg::DayImportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayImportDlg)
{
    ui->setupUi(this);
}

DayImportDlg::DayImportDlg(DbUtil *db ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayImportDlg)
{
    ui->setupUi(this);
    this->db_util = db;
    ui->pro_cc->setRange(0, 100);
    ui->pro_cc->setValue(0);
    ui->pro_cj->setRange(0, 100);
    ui->pro_cj->setValue(0);
    ui->pro_zj->setRange(0, 100);
    ui->pro_zj->setValue(0);
    path = cset.getImportPath();
    if (path.isEmpty()) {
        path = "C:\\";
        cset.setImportPath(path);
    }
    f_dlg = new QFileDialog(this, QStringLiteral("选择xls文件"), path, QStringLiteral("xls文件(*.xls)"));
    f_dlg->setAcceptMode(QFileDialog::AcceptOpen);
    f_dlg->setFileMode(QFileDialog::ExistingFile);
}

DayImportDlg::~DayImportDlg()
{
    delete ui;
}

void DayImportDlg::closeEvent(QCloseEvent *e)
{
    hide();
    QWidget *p = (QWidget *)parent();
    p->hide();
    e->ignore();
}

void DayImportDlg::on_tb_cj_clicked()
{
    f_dlg->restoreState(f_array);
    if (f_dlg->exec() == QDialog::Accepted) {
        QStringList filePaths = f_dlg->selectedFiles();
        if (filePaths.size() == 1) {
            cj_path = filePaths.at(0);
            ui->le_cj->setText(cj_path);
        }
    }
    f_array = f_dlg->saveState();
}

void DayImportDlg::on_pb_cj_clicked()
{
    QString le_text = ui->le_cj->text();
    if (le_text != cj_path) {
        cj_path = le_text;
    }
    QFileInfo xls_info(cj_path);
    if (xls_info.exists()) {
        bool ok = db_util->insertDealList(cj_path, ui->pro_cj);
        if (ok) {
            this->path = xls_info.absoluteDir().absolutePath();
            cset.setImportPath(this->path);
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("导入成功"),
                QMessageBox::Yes, QMessageBox::Yes);
        }
    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("文件不存在！，请重新选择"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void DayImportDlg::on_tb_cc_clicked()
{
    f_dlg->restoreState(f_array);
    if (f_dlg->exec() == QDialog::Accepted) {
        QStringList filePaths = f_dlg->selectedFiles();
        if (filePaths.size() == 1) {
            cc_path = filePaths.at(0);
            ui->le_cc->setText(cc_path);
        }
    }
    f_array = f_dlg->saveState();
}

void DayImportDlg::on_pb_cc_clicked()
{
    QString le_text = ui->le_cc->text();
    if (le_text != cc_path) {
        cc_path = le_text;
    }
    QFileInfo xls_info(cj_path);
    if (xls_info.exists()) {
        bool ok = db_util->insertHoldList(cc_path, ui->pro_cc);
        if (ok) {
            this->path = xls_info.absoluteDir().absolutePath();
            cset.setImportPath(this->path);
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("导入成功"),
                QMessageBox::Yes, QMessageBox::Yes);
        }
    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("文件不存在！，请重新选择"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void DayImportDlg::on_tb_zj_clicked()
{
    f_dlg->restoreState(f_array);
    if (f_dlg->exec() == QDialog::Accepted) {
        QStringList filePaths = f_dlg->selectedFiles();
        if (filePaths.size() == 1) {
            zj_path = filePaths.at(0);
            ui->le_zj->setText(zj_path);
        }
    }
    f_array = f_dlg->saveState();
}

void DayImportDlg::on_pb_zj_clicked()
{
    QString le_text = ui->le_zj->text();
    if (le_text != zj_path) {
        zj_path = le_text;
    }
    QFileInfo xls_info(zj_path);
    if (xls_info.exists()) {
        bool ok = db_util->insertFundList(zj_path, ui->pro_zj);
        if (ok) {
            this->path = xls_info.absoluteDir().absolutePath();
            cset.setImportPath(this->path);
            QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("导入成功"),
                QMessageBox::Yes, QMessageBox::Yes);
        }
    } else {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("文件不存在！，请重新选择"),
            QMessageBox::Yes, QMessageBox::Yes);
    }
}

void DayImportDlg::on_pb_cancel_clicked()
{
    close();
}
