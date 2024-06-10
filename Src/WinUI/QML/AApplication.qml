import QtQuick 2.3
import Aproch.WinUI 1.0

QtObject {
    id: app
    Connections {
        target: AWinUITheme
        function onDarkModeChanged() {
            // AWinUITheme.saveDarkMode()
            console.log("dark mode changed")
        }
    }
    Connections {
        target: AWinUIApplication
        function onIsUseSystemAppBarChanged() {
            // AWinUITheme.saveIsUseSystemAppBar()
            console.log("use system app bar")
        }
    }
    Component.onCompleted: {

        console.log('fucking')

        // 初始化qml应用程序
        AWinUIApplication.init(app, Qt.locale())
    }

    property alias hotKeys: object_hotkey

    QtObject {
        id: object_hotkey
        AHotkey {
            name: qsTr("Quit")
            sequence: "Ctrl+Q"
            onActivated: {
                // ARouter.exit()
                console.log('quit')
            }
        }
    }
}
