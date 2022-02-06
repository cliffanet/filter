QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    graphpaint.cpp \
    main.cpp \
    mainwindow.cpp \
    moddata.cpp \
    wnddata.cpp \
    wndimport.cpp

HEADERS += \
    ../filtlib/filter_avg.h \
    ../filtlib/filter_avg2.h \
    ../filtlib/filter_avgdeg.h \
    ../filtlib/filter_ltsqrt.h \
    ../filtlib/ring.h \
    ../filtlib/filter.h \
    graphpaint.h \
    mainwindow.h \
    moddata.h \
    wnddata.h \
    wndimport.h

FORMS += \
    mainwindow.ui \
    wnddata.ui \
    wndimport.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
