TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

CONFIG += c++11

include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    README.md

