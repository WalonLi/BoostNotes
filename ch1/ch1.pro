

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -L/usr/lib/boost_1_57_0 -lboost_system -lboost_thread -lboost_regex
INCLUDEPATH += /usr/include/boost_1_57_0

include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    README.md

