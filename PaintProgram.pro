#-------------------------------------------------
#
# Project created by QtCreator 2021-09-06T10:47:06
#
#-------------------------------------------------

QT       += core gui printsupport quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PaintProgram
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
        mainwindow.cpp \
    scribblearea.cpp \
    submenu.cpp \
    gradientcolorinputdialog.cpp \
    toolsethandling.cpp \
    shapes/rectangle.cpp \
    shapes/ellipse.cpp \
    shapes/squircle.cpp \
    shapes/freehandline.cpp \
    shapes/convexpolygon.cpp \
    shapes/straightline.cpp \
    shapes/lineargradientshape.cpp \
    shapes/conicalgradientshape.cpp \
    shapes/radialgradientshape.cpp \
    shapes/textbox.cpp \
    shapes/paintbucket.cpp

HEADERS += \
        mainwindow.h \
    scribblearea.h \
    submenu.h \
    gradientcolorinputdialog.h \
    toolsethandling.h \
    shapes/rectangle.h \
    shapes/ellipse.h \
    shapes/squircle.h \
    shapes/freehandline.h \
    shapes/convexpolygon.h \
    shapes/straightline.h \
    shapes/lineargradientshape.h \
    shapes/conicalgradientshape.h \
    shapes/radialgradientshape.h \
    shapes/textbox.h \
    shapes/paintbucket.h

FORMS += \
        mainwindow.ui

DISTFILES +=

RESOURCES += \
    tooliconimages.qrc


