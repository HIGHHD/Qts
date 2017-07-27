#include "accountitem.h"

AccountItem::AccountItem(QWidget *parent)
    : QWidget(parent)
{
    mouse_press = false;
    account_number = new QLabel();
    delede_button = new QPushButton();
    QPixmap pixmap(":/delsmallgray");
    delede_button->setIcon(pixmap);
    delede_button->setIconSize(pixmap.size());
    delede_button->setStyleSheet("background:transparent;");
    connect(delede_button, SIGNAL(clicked()), this, SLOT(removeAccount()));
    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addWidget(account_number);
    main_layout->addStretch();
    main_layout->addWidget(delede_button);
    main_layout->setContentsMargins(5, 5, 5, 5);
    main_layout->setSpacing(5);
    setLayout(main_layout);
    installEventFilter(this);
    account_number->setFont(QFont("arial", 10, QFont::Normal));
    account_number->setStyleSheet("color:#666666");
}
AccountItem::~AccountItem()
{
}
void AccountItem::setAccountNumber(QString text)
{
    account_number->setText(text);
}
QString AccountItem::getAccountNumber()
{
    QString account_number_text = account_number->text();
    return account_number_text;
}
void AccountItem::removeAccount()
{
    QString account_number_text = account_number->text();
    emit removeAccount(account_number_text);
}
void AccountItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mouse_press = true;
    }
}
void AccountItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (mouse_press) {
        emit showAccount(account_number->text());
        mouse_press = false;
    }
}
bool AccountItem::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Enter) {
        QPixmap pixmap(":/delsmall");
        delede_button->setIcon(pixmap);
        account_number->setFont(QFont("arial", 12, QFont::Normal));
        account_number->setStyleSheet("color:#FFFFFF");
    } else if (event->type() == QEvent::Leave) {
        QPixmap pixmap(":/delsmallgray");
        delede_button->setIcon(pixmap);
        account_number->setFont(QFont("arial", 10, QFont::Normal));
        account_number->setStyleSheet("color:#666666");
    }
    return QWidget::eventFilter(obj, event);
}
