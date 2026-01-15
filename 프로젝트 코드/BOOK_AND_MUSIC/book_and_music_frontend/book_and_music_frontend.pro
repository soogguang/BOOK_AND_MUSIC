QT += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = book_and_music
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    bookwidget.cpp

HEADERS += \
    mainwindow.h \
    bookwidget.h
