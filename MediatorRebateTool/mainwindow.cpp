#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QAction *helpAction = ui->menuBar->addAction(QStringLiteral("退出系统"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(quitSystem()));

    initSubWin();
}

MainWindow::MainWindow(DbUtil *db ,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QAction *helpAction = ui->menuBar->addAction(QStringLiteral("退出系统"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(quitSystem()));

    this->db_util = db;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initLogin(DlgLogin *dlg)
{
    plogin = dlg;
    connect(plogin, &DlgLogin::doLogin, this, &MainWindow::getLogin);
}

void MainWindow::initSubWin()
{
    mdiarea = new QMdiArea(this);
    mdiarea->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setCentralWidget(mdiarea);
    pdeptMng = new DlgDeptMng(db_util, mdiarea);
    clientInfoMng = new ClientInfoMng(db_util, mdiarea);
    agentInfoMng = new AgentInfoMng(db_util, mdiarea);
    brokerInfoMng = new BrokerInfoMng(db_util, mdiarea);
    rcmInfoMng = new RcmInfoMng(db_util, mdiarea);
    relationMng = new RelationMng(db_util, agentInfoMng, rcmInfoMng, brokerInfoMng, clientInfoMng, mdiarea);
    userInfoMng = new UserInfoMng(db_util, loginUser, mdiarea);

    dayImportDlg = new DayImportDlg(db_util, mdiarea);
    crmExportDlg = new CrmExportDlg(db_util, mdiarea);
    dealExportDlg = new DealExportDlg(db_util, mdiarea);

    mdiarea->addSubWindow(userInfoMng);
    mdiarea->addSubWindow(pdeptMng);
    mdiarea->addSubWindow(clientInfoMng);
    mdiarea->addSubWindow(agentInfoMng);
    mdiarea->addSubWindow(brokerInfoMng);
    mdiarea->addSubWindow(rcmInfoMng);
    mdiarea->addSubWindow(relationMng);

    subDayImport = mdiarea->addSubWindow(dayImportDlg);
    mdiarea->addSubWindow(crmExportDlg);
    mdiarea->addSubWindow(dealExportDlg);

    mdiarea->show();
    mdiarea->closeAllSubWindows();

    connect(ui->actclientMng, &QAction::triggered, this, &MainWindow::clientManage);
    connect(ui->actagentMng, &QAction::triggered, this, &MainWindow::agentManage);
    connect(ui->actmediatorMng, &QAction::triggered, this, &MainWindow::mediatorManage);
    connect(ui->actrecomMng, &QAction::triggered, this, &MainWindow::rcmManage);
    connect(ui->actuser, &QAction::triggered, this, &MainWindow::userManage);
    connect(ui->actdept, &QAction::triggered, this, &MainWindow::deptManage);
    connect(ui->actrelaMng, &QAction::triggered, this, &MainWindow::relationManage);

    connect(ui->acDayImport, &QAction::triggered, this, &MainWindow::dayImportManage);
    connect(ui->acCrmExport, &QAction::triggered, this, &MainWindow::crmExportManage);
    connect(ui->acInfoExport, &QAction::triggered, this, &MainWindow::dealExportManage);

    connect(pdeptMng, &DlgDeptMng::deptHaveChanged, userInfoMng, &UserInfoMng::deptHaveChanged);
    connect(pdeptMng, &DlgDeptMng::deptHaveChanged, rcmInfoMng, &RcmInfoMng::deptHaveChanged);
    connect(pdeptMng, &DlgDeptMng::deptHaveChanged, brokerInfoMng, &BrokerInfoMng::deptHaveChanged);
    connect(pdeptMng, &DlgDeptMng::deptHaveChanged, agentInfoMng, &AgentInfoMng::deptHaveChanged);

    connect(pdeptMng, &DlgDeptMng::shouldUpdateDept, userInfoMng, &UserInfoMng::updateInsDept);
    connect(pdeptMng, &DlgDeptMng::shouldUpdateDept, rcmInfoMng, &RcmInfoMng::updateInsDept);
    connect(pdeptMng, &DlgDeptMng::shouldUpdateDept, brokerInfoMng, &BrokerInfoMng::updateInsDept);
    connect(pdeptMng, &DlgDeptMng::shouldUpdateDept, agentInfoMng, &AgentInfoMng::updateInsDept);

    connect(pdeptMng, &DlgDeptMng::deptHaveChanged, dealExportDlg, &DealExportDlg::infoChanged);
    connect(agentInfoMng, &AgentInfoMng::relaInfoDelete, dealExportDlg, &DealExportDlg::infoChanged);
    connect(agentInfoMng, &AgentInfoMng::relaInfoUpdate, dealExportDlg, &DealExportDlg::infoChanged);
}

void MainWindow::quitSystem()
{
    close();
}

void MainWindow::userManage()
{
    userInfoMng->setVisible(true);
    userInfoMng->showMaximized();
}
void MainWindow::deptManage()
{
    if (loginUser.userType == 1) {
        pdeptMng->setVisible(true);
        pdeptMng->showMaximized();
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("无此权限！"),
                QMessageBox::Yes, QMessageBox::Yes);
    }
}
void MainWindow::clientManage()
{
    if (loginUser.userType == 1) {
        clientInfoMng->setVisible(true);
        clientInfoMng->showMaximized();
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("无此权限！"),
                QMessageBox::Yes, QMessageBox::Yes);
    }
}

void MainWindow::agentManage()
{
    if (loginUser.userType == 1) {
        agentInfoMng->setVisible(true);
        agentInfoMng->showMaximized();
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("无此权限！"),
                QMessageBox::Yes, QMessageBox::Yes);
    }
}

void MainWindow::rcmManage()
{
    if (loginUser.userType == 1) {
        rcmInfoMng->setVisible(true);
        rcmInfoMng->showMaximized();
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("无此权限！"),
                QMessageBox::Yes, QMessageBox::Yes);
    }
}

void MainWindow::mediatorManage()
{
    if (loginUser.userType == 1) {
        brokerInfoMng->setVisible(true);
        brokerInfoMng->showMaximized();
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("无此权限！"),
                QMessageBox::Yes, QMessageBox::Yes);
    }
}

void MainWindow::relationManage()
{
    if (loginUser.userType == 1) {
        relationMng->setVisible(true);
        relationMng->showMaximized();
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("无此权限！"),
                QMessageBox::Yes, QMessageBox::Yes);
    }
}

void MainWindow::dayImportManage()
{
    if (loginUser.userType == 1) {
        dayImportDlg->setVisible(true);
        dayImportDlg->showNormal();
        mdiarea->setActiveSubWindow(subDayImport);
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("无此权限！"),
                QMessageBox::Yes, QMessageBox::Yes);
    }
}

void MainWindow::crmExportManage()
{
    crmExportDlg->setVisible(true);
    crmExportDlg->showMaximized();
}

void MainWindow::dealExportManage()
{
    dealExportDlg->setVisible(true);
    dealExportDlg->showMaximized();
}

void MainWindow::getLogin(UserInfo &user)
{
    this->loginUser.id = user.id;
    this->loginUser.name = user.name;
    this->loginUser.deptId = user.deptId;
    this->loginUser.deptName = user.deptName;
    this->loginUser.userType = user.userType;
    this->loginUser.password = user.password;

    initSubWin();

    showMaximized();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    //qDebug() << "quit application!" ;
    int result = QMessageBox::information(this, QStringLiteral("确认退出"), QStringLiteral("确认要退出系统吗？"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (result == QMessageBox::Yes) {
        e->accept();
    } else {
        e->ignore();
    }
}

