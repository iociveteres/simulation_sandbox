QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Action.cpp \
    Ball.cpp \
    BaseEntity.cpp \
    MainWindow.cpp \
    ModelConstants.cpp \
    Player.cpp \
    PlayerAI.cpp \
    PlayerRole.cpp \
    PlayerWorldModel.cpp \
    PrefferedPoint.cpp \
    RenderArea.cpp \
    Velocity.cpp \
    World.cpp \
    main.cpp

HEADERS += \
    Action.h \
    Ball.h \
    BaseEntity.h \
    MainWindow.h \
    ModelConstants.h \
    Player.h \
    PlayerAI.h \
    PlayerRole.h \
    PlayerWorldModel.h \
    PrefferedPoint.h \
    RenderArea.h \
    Velocity.h \
    World.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
