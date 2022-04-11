QT += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += force_debug_info

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BgmPlayer.cpp \
    FileManager.cpp \
    GameMainWindow.cpp \
    GameTimer.cpp \
    HomeWindow.cpp \
    Pig.cpp \
    PigSty.cpp \
    PushButtonToSty.cpp \
    SettingsWindow.cpp \
    StyDetailWindow.cpp \
    TradeRecordWindow.cpp \
    main.cpp

HEADERS += \
    BgmPlayer.h \
    FileManager.hpp \
    GameMainWindow.hpp \
    GameTimer.hpp \
    HomeWindow.hpp \
    Pig.hpp \
    PigSty.hpp \
    PushButtonToSty.hpp \
    SettingsWindow.hpp \
    StyDetailWindow.hpp \
    TradeRecordWindow.hpp

FORMS += \
    GameMainWindow.ui \
    HomeWindow.ui \
    SettingsWindow.ui \
    StyDetailWindow.ui \
    TradeRecordWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
