#-------------------------------------------------
#
# Project created by QtCreator 2018-05-12T18:42:51
#
#-------------------------------------------------

QT       += core gui printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    delegates/SlidingStackedWidget.cpp \
    dialogs/SettingsDialog.cpp \
    dialogs/WieagandReaderDialog.cpp \
    forms/HistoryForm.cpp \
    forms/ProductsForm.cpp \
    forms/RegisterForm.cpp \
    forms/StatusForm.cpp \
    forms/UniteForm.cpp \
    forms/DashboardForm.cpp\
    models/ProxyModel.cpp \
    forms/AdditionalSettingsForm.cpp \
    delegates/TransactionsFrame.cpp \
    models/DashboardProxyModel.cpp \
    models/CustomSqlTableModel.cpp \
    forms/SalesForm.cpp \
    models/ProductTypesProxyModel.cpp \
    models/ProductsProxyModel.cpp \
    models/CartProxyModel.cpp

HEADERS += \
        MainWindow.h \
    delegates/SlidingStackedWidget.h \
    dialogs/SettingsDialog.h \
    dialogs/WieagandReaderDialog.h \
    forms/HistoryForm.h \
    forms/ProductsForm.h \
    forms/RegisterForm.h \
    forms/StatusForm.h \
    forms/UniteForm.h \
    forms/DashboardForm.h \
    forms/AdditionalSettingsForm.h\
    models/ProxyModel.h \
    delegates/TransactionsFrame.h \
    models/DashboardProxyModel.h \
    models/CustomSqlTableModel.h \
    forms/SalesForm.h \
    models/ProductTypesProxyModel.h \
    models/ProductsProxyModel.h \
    models/CartProxyModel.h

FORMS += \
        MainWindow.ui \
    dialogs/SettingsDialog.ui \
    dialogs/WieagandReaderDialog.ui \
    forms/HistoryForm.ui \
    forms/ProductsForm.ui \
    forms/RegisterForm.ui \
    forms/StatusForm.ui \
    forms/UniteForm.ui \
    forms/DashboardForm.ui \
    forms/AdditionalSettingsForm.ui \
    delegates/TransactionsFrame.ui \
    forms/SalesForm.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

RESOURCES += \
    clientresources.qrc
