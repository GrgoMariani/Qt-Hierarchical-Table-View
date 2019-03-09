QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    HTV/customconstants.h \
    HTV/ht_headerview.h \
    HTV/ht_item.h \
    HTV/ht_model.h

SOURCES += \
    HTV/ht_headerview.cpp \
    HTV/ht_item.cpp \
    HTV/ht_model.cpp \
    main.cpp

CONFIG += qt c++11
