#ifndef DEALEXPORTPOP_H
#define DEALEXPORTPOP_H

#include <QDialog>
#include <QStringList>
#include <QMap>

namespace Ui {
class DealExportPop;
}

class DealExportPop : public QDialog
{
    Q_OBJECT

public:
    explicit DealExportPop(int saveType, QWidget *parent = 0);
    ~DealExportPop();

private slots:
    void on_btn_save_clicked();
    void on_btn_cancel_clicked();

signals:
    void saveParams(QMap<QString, QStringList> &map);

private:
    Ui::DealExportPop *ui;
    int ty;
};

#endif // DEALEXPORTPOP_H
