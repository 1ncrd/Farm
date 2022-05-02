QT += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += force_debug_info

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ArchiveItem.cpp \
    ArchiveWindow.cpp \
    BgmPlayer.cpp \
    CreateWindow.cpp \
    FileManager.cpp \
    GameInfo.cpp \
    GameMainWindow.cpp \
    GameTimer.cpp \
    HomeWindow.cpp \
    MyListWidget.cpp \
    MyPushButton.cpp \
    MyRandom.cpp \
    Pig.cpp \
    PigSoldAmount.cpp \
    PigSty.cpp \
    PigStyManager.cpp \
    PushButtonToSty.cpp \
    QuarantinePigSty.cpp \
    QuarantineStyWindow.cpp \
    SettingWindow.cpp \
    StyDetailWindow.cpp \
    TradeRecordWindow.cpp \
    main.cpp

HEADERS += \
    ArchiveItem.hpp \
    ArchiveWindow.hpp \
    BgmPlayer.hpp \
    CreateWindow.hpp \
    FileManager.hpp \
    GameInfo.hpp \
    GameMainWindow.hpp \
    GameTimer.hpp \
    HomeWindow.hpp \
    MyListWidget.hpp \
    MyPushButton.hpp \
    MyRandom.hpp \
    Pig.hpp \
    PigSoldAmount.hpp \
    PigSty.hpp \
    PigStyManager.hpp \
    PushButtonToSty.hpp \
    QuarantinePigSty.hpp \
    QuarantineStyWindow.hpp \
    SettingWindow.hpp \
    StyDetailWindow.hpp \
    TradeRecordWindow.hpp

FORMS += \
    ArchiveItem.ui \
    ArchiveWindow.ui \
    CreateWindow.ui \
    GameMainWindow.ui \
    HomeWindow.ui \
    QuarantineStyWindow.ui \
    SettingWindow.ui \
    StyDetailWindow.ui \
    TradeRecordWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc

DISTFILES +=
