#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QEvent>

class AccountItem : public QWidget
{
    Q_OBJECT
public:
    explicit AccountItem(QWidget *parent = 0);
    ~AccountItem();
    void setAccountNumber(QString text);
    QString getAccountNumber();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
private:
    bool mouse_press;
    QLabel *account_number;
    QPushButton *delede_button;
signals:
    void showAccount(QString );
    void removeAccount(QString );
public slots:
    void removeAccount();
};

#endif // ACCOUNTITEM_H
