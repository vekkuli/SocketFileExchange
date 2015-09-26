#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T11:25:05
#
#-------------------------------------------------

QT += network widgets

TARGET = FileExchange
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileserver.cpp \
    fileexchangeitem.cpp \
    fileexchangeitemdelegate.cpp \
    fileexchangetablemodel.cpp \
    fileoperationsockets.cpp

HEADERS  += mainwindow.h \
    fileserver.h \
    utildefines.h \
    fileexchangeitem.h \
    fileexchangeitemdelegate.h \
    fileexchangetablemodel.h \
    fileoperationsockets.h

FORMS    += mainwindow.ui
