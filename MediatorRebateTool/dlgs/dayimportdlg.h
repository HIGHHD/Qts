#ifndef DAYIMPORTDLG_H
#define DAYIMPORTDLG_H

#include <QDialog>
#include <QList>
#include <QDebug>
#include <QFileDialog>
#include "dbutil.h"
#include "struct.h"
#include "paramsettings.h"
#include <QMessageBox>
#include <QFile>
#include <QByteArray>

namespace Ui {
class DayImportDlg;
}

class DayImportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DayImportDlg(QWidget *parent = 0);
    explicit DayImportDlg(DbUtil *db, QWidget *parent = 0);
    ~DayImportDlg();
    void closeEvent(QCloseEvent *e);

private slots:
    void on_tb_cj_clicked();
    void on_pb_cj_clicked();
    void on_tb_cc_clicked();
    void on_pb_cc_clicked();
    void on_tb_zj_clicked();
    void on_pb_zj_clicked();
    void on_pb_cancel_clicked();

private:
    Ui::DayImportDlg *ui;
    CParamSettings cset;
    DbUtil *db_util;
    QFileDialog *f_dlg;
    QString path;
    QByteArray f_array;
    QString cj_path;
    QString cc_path;
    QString zj_path;
};

#endif // DAYIMPORTDLG_H
