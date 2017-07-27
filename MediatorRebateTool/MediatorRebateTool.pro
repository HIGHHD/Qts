#-------------------------------------------------
#
# Project created by QtCreator 2017-05-22T15:27:37
#
#-------------------------------------------------

QT += core gui
QT += sql
QT += axcontainer

RESOURCES += imageres.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MediatorRebateTool
TEMPLATE = app

INCLUDEPATH += ./

SOURCES += main.cpp\
    dlgs/agentinfomng.cpp \
    dlgs/agentinfopop.cpp \
    dlgs/agentrelapop.cpp \
    dlgs/brokerinfomng.cpp \
    dlgs/brokerinfopop.cpp \
    dlgs/brokerrelapop.cpp \
    dlgs/brokerrelastatepop.cpp \
    dlgs/clientinfomng.cpp \
    dlgs/clientinfopop.cpp \
    dlgs/dlgdeptmng.cpp \
    dlgs/dlgdeptpop.cpp \
    dlgs/rcminfomng.cpp \
    dlgs/rcminfopop.cpp \
    dlgs/rcmrelapop.cpp \
    dlgs/relationmng.cpp \
    accountitem.cpp \
    dbutil.cpp \
    dlglogin.cpp \
    dlgserver.cpp \
    excelengine.cpp \
    fileecrypt.cpp \
    mainwindow.cpp \
    mycombox.cpp \
    paramsettings.cpp \
    dlgs/userinfomng.cpp \
    dlgs/userinfopop.cpp \
    dlgs/userpasswordpop.cpp \
    dlgs/dayimportdlg.cpp \
    dlgs/crmexportdlg.cpp \
    dlgs/dealexportdlg.cpp \
    dlgs/dealexportpop.cpp

FORMS += \
    dlgs/agentinfomng.ui \
    dlgs/agentinfopop.ui \
    dlgs/agentrelapop.ui \
    dlgs/brokerinfomng.ui \
    dlgs/brokerinfopop.ui \
    dlgs/brokerrelapop.ui \
    dlgs/brokerrelastatepop.ui \
    dlgs/clientinfomng.ui \
    dlgs/clientinfopop.ui \
    dlgs/dlgdeptmng.ui \
    dlgs/dlgdeptpop.ui \
    dlgs/rcminfomng.ui \
    dlgs/rcminfopop.ui \
    dlgs/rcmrelapop.ui \
    dlgs/relationmng.ui \
    dlglogin.ui \
    dlgserver.ui \
    mainwindow.ui \
    dlgs/userinfomng.ui \
    dlgs/userinfopop.ui \
    dlgs/userpasswordpop.ui \
    dlgs/dayimportdlg.ui \
    dlgs/crmexportdlg.ui \
    dlgs/dealexportdlg.ui \
    dlgs/dealexportpop.ui

HEADERS += \
    dlgs/agentinfomng.h \
    dlgs/agentinfopop.h \
    dlgs/agentrelapop.h \
    dlgs/brokerinfomng.h \
    dlgs/brokerinfopop.h \
    dlgs/brokerrelapop.h \
    dlgs/brokerrelastatepop.h \
    dlgs/clientinfomng.h \
    dlgs/clientinfopop.h \
    dlgs/dlgdeptmng.h \
    dlgs/dlgdeptpop.h \
    dlgs/rcminfomng.h \
    dlgs/rcminfopop.h \
    dlgs/rcmrelapop.h \
    dlgs/relationmng.h \
    accountitem.h \
    dbutil.h \
    dlglogin.h \
    dlgserver.h \
    excelengine.h \
    fileencrypt.h \
    mainwindow.h \
    mycombox.h \
    paramsettings.h \
    struct.h \
    dlgs/userinfomng.h \
    dlgs/userinfopop.h \
    dlgs/userpasswordpop.h \
    dlgs/dayimportdlg.h \
    dlgs/crmexportdlg.h \
    dlgs/dealexportdlg.h \
    dlgs/dealexportpop.h

