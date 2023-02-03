QT       += core gui
QT       += core gui openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    block.cpp \
    building.cpp \
    camera.cpp \
    defensebuilding.cpp \
    extractionbuilding.cpp \
    group.cpp \
    iplayer.cpp \
    main.cpp \
    material.cpp \
    materiallibrary.cpp \
    menu.cpp \
    object3d.cpp \
    player.cpp \
    skybox.cpp \
    unit.cpp \
    widget.cpp

HEADERS += \
    block.h \
    building.h \
    camera.h \
    defensebuilding.h \
    extractionbuilding.h \
    group.h \
    iplayer.h \
    material.h \
    materiallibrary.h \
    menu.h \
    object3d.h \
    player.h \
    skybox.h \
    unit.h \
    widget.h \
    worldenginebase.h

win32: LIBS += -lopengl32 #подключение на windows

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    models.qrc \
    shaders.qrc \
    textures.qrc

FORMS += \
    menu.ui

DISTFILES +=
