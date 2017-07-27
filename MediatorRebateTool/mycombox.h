#ifndef MYCOMBOX_H
#define MYCOMBOX_H

#include <QComboBox>
#include <QFocusEvent>
#include <QDebug>
#include "dlglogin.h"
#include <QLabel>
#include <QList>
#include "struct.h"
#include "dlglogin.h"

class MyComBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MyComBox(QWidget *parent = 0);
    void focusOutEvent(QFocusEvent * e);
    void focusInEvent(QFocusEvent * e);
signals:
    void checkUser();

public slots:

};

#endif // MYCOMBOX_H
