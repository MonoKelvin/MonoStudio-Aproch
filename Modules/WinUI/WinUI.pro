TEMPLATE = lib
TARGET = AprochWinUI
QT += core qml quick
CONFIG += plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = Aproch.WinUI

# Input
SOURCES += \
        src/AWinUIApplication.cpp \
        src/AWinUIPlugin.cpp

HEADERS += \
        src/AWinUIApplication.h \
        src/AWinUIPlugin.h

INCLUDEPATH += \
        $$PWD/src/

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

RESOURCES += \
    WinUI.qrc

# copy files into install path
qmlInstallPath = $$[QT_INSTALL_QML]/$$replace(uri, \., /)

qmldir.files = qmldir
qmldir.path = $$qmlInstallPath

designer.files = $$PWD/designer/winui.metainfo
designer.path = $$qmlInstallPath/designer

qml_files.files = $$PWD/qml/*.qml
qml_files.path = $$qmlInstallPath

INSTALLS += designer qmldir qml_files

CONFIG += qmltypes
QML_IMPORT_NAME = Aproch.WinUI
QML_IMPORT_MAJOR_VERSION = 1
