QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbmanager.cpp \
    guestmanagement.cpp \
    guestmanager.cpp \
    logger.cpp \
    login.cpp \
    main.cpp \
    mainwidget.cpp \
    querymanagement.cpp \
    register.cpp \
    reservationmanagement.cpp \
    reservationmanager.cpp \
    roommanagement.cpp \
    roommanager.cpp \
    splashscreen.cpp \
    usermanager.cpp

HEADERS += \
    dbmanager.h \
    guestmanagement.h \
    guestmanager.h \
    logger.h \
    login.h \
    mainwidget.h \
    querymanagement.h \
    register.h \
    reservationmanagement.h \
    reservationmanager.h \
    roommanagement.h \
    roommanager.h \
    splashscreen.h \
    usermanager.h

FORMS += \
    guestmanagement.ui \
    login.ui \
    mainwidget.ui \
    querymanagement.ui \
    register.ui \
    reservationmanagement.ui \
    roommanagement.ui \
    splashscreen.ui\
    reservationmanagement.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc \
    Resources.qrc

DISTFILES += \
    reservationmanagement.txt
