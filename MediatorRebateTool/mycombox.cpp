#include "mycombox.h"

MyComBox::MyComBox(QWidget *parent) :
    QComboBox(parent)
{
}

void MyComBox::focusOutEvent(QFocusEvent * e)
{
    emit checkUser();
}

void MyComBox::focusInEvent(QFocusEvent * e)
{
    emit checkUser();
}
