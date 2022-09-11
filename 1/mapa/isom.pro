TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    mapprovider.cpp

RESOURCES += qml.qrc \
    textures-0.qrc \
    other-0.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    mapprovider.h

