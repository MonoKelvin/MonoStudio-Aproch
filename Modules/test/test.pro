TARGET = test
TEMPLATE = app
QT += core qml quick
CONFIG += c++11

SOURCES += \
        $$PWD/main.cpp

RESOURCES += \
    $$PWD/qml.qrc

QML_IMPORT_PATH += \
    E:/Project/Code/MonoStudio/Aproch/Modules/test/build/Desktop_Qt_6_6_2_MSVC2019_64bit-Debug/debug
