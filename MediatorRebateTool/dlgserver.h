#ifndef DLGSERVER_H
#define DLGSERVER_H

#include <QDialog>
#include "struct.h"
#include <QList>
#include <QDebug>

namespace Ui {
class DlgServer;
}

class DlgServer : public QDialog
{
    Q_OBJECT

public:
    explicit DlgServer(QWidget *parent = 0);
    explicit DlgServer(QList<StDb>* dbList, int index, int curIndex, QWidget *parent = 0);
    ~DlgServer();
    StDb db_info;
    QList<StDb> *db_list;
    //当前点击的列表行，若是addserver信号过来，index为-1,editserver信号过来是大于0的
    int index;
    //已经选择的列表行
    int curIndex;

signals:
    void dbInfoSaved(QList<StDb> &db_list, int index);
    void dbInfoSavedAndSelected(QList<StDb> &db_list, int index);

private slots:
    void on_cancel_btn_clicked();

    void on_save_set_btn_clicked();

    void on_save_btn_clicked();

private:
    Ui::DlgServer *ui;
    bool isExitDbInfo(StDb &db_info);

};

#endif // DLGSERVER_H
