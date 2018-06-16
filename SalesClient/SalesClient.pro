#-------------------------------------------------
#
# Project created by QtCreator 2018-06-16T11:32:02
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SalesClient
TEMPLATE = app

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
        MainWindow.cp \
    delegates/SlidingStackedWidget.cpp \
    dialogs/SettingsDialog.cpp \
    dialogs/WieagandReaderDialog.cpp \
    models/CustomSqlTableModel.cpp


HEADERS += \
        MainWindow.h \
    delegates/SlidingStackedWidget.h \
    dialogs/SettingsDialog.h \
    dialogs/WieagandReaderDialog.h \
    models/CustomSqlTableModel.h


FORMS += \
        MainWindow.ui \
    dialogs/SettingsDialog.ui \
    dialogs/WieagandReaderDialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

RESOURCES += \
    clientresources.qrc
