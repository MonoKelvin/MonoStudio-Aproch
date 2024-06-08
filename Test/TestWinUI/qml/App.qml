//import QtQuick
//import QtQuick.Controls
//import QWindowKit 1.0
//
//QtObject {
//    id: root
//
//    property var controlWindow: Window {
//        id:window
//        width: 1080
//        height: 640
//        title: "Mono Dream"
//        visible: false // We hide it first, so we can move the window to our desired position silently.
//        Component.onCompleted: {
//            windowAgent.setup(window)
//            windowAgent.setSystemButton()
//            window.visible = true
//        }
//        WindowAgent {
//            id: windowAgent
//            // ...
//        }
//
//        Item {
//            id: myTitleBar
//            Item { id: someControl1 }
//            Item { id: someControl2 }
//            Item { id: someControl3 }
//        }
//    }
//}

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
//import Qt.labs.platform 1.1
import QWindowKit 1.0

Window {
    id: window
    width: 800
    height: 600
    color: darkStyle.windowBackgroundColor
    title: qsTr("Hello, world!")
    Component.onCompleted: {
        windowAgent.setup(window)
        window.visible = true
    }

    QtObject {
        id: lightStyle
    }

    QtObject {
        id: darkStyle
        readonly property color windowBackgroundColor: "#1E1E1E"
    }

    Timer {
        interval: 100
        running: true
        repeat: true
        onTriggered: timeLabel.text = Qt.formatTime(new Date(), "hh:mm:ss")
    }

    WindowAgent {
        id: windowAgent
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        //onClicked: contextMenu.open()
    }

    Rectangle {
        id: titleBar 
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 32
        color: darkStyle.windowBackgroundColor
        //color: window.active ? "#3C3C3C" : "#505050"
        Component.onCompleted: windowAgent.setTitleBar(titleBar)

        Image {
            id: iconButton
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 10
            }
            width: 18
            height: 18
            mipmap: true
            source: "qrc:///app/example.png"
            Component.onCompleted: windowAgent.setSystemButton(WindowAgent.WindowIcon, iconButton)
        }

        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                left: iconButton.right
                leftMargin: 10
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: window.title
            font.pixelSize: 14
            color: "#ECECEC"
        }

        Row {
            anchors {
                top: parent.top
                right: parent.right
            }
            height: parent.height

            Button {
                id: minButton
                width: 54
                height: parent.height
                text: "_"
                onClicked: window.showMinimized()
                Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Minimize, minButton)
            }

            Button {
                id: maxButton
                width: 54
                height: parent.height
                text: window.visibility === Window.Maximized ? "h" : "m"
                onClicked: {
                    if (window.visibility === Window.Maximized) {
                        window.showNormal()
                    } else {
                        window.showMaximized()
                    }
                }
                Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Maximize, maxButton)
            }

            Button {
                id: closeButton
                width: 54
                height: parent.height
                text: "x"
                background: Rectangle {
                    color: {
                        if (!closeButton.enabled) {
                            return "gray";
                        }
                        if (closeButton.pressed) {
                            return "#e81123";
                        }
                        if (closeButton.hovered) {
                            return "#e81123";
                        }
                        return "transparent";
                    }
                }
                onClicked: window.close()
                Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Close, closeButton)
            }
        }
    }

    Label {
        id: timeLabel
        anchors.centerIn: parent
        font {
            pointSize: 75
            bold: true
        }
        color: "#FEFEFE"
    }

    /*Menu {
        id: contextMenu

        Menu {
            id: themeMenu
            title: qsTr("Theme")

            MenuItemGroup {
                id: themeMenuGroup
                items: themeMenu.items
            }

            MenuItem {
                text: qsTr("Light")
                checkable: true
                onTriggered: windowAgent.setWindowAttribute("dark-mode", false)
            }

            MenuItem {
                text: qsTr("Dark")
                checkable: true
                onTriggered: windowAgent.setWindowAttribute("dark-mode", true)
            }
        }

        Menu {
            id: specialEffectMenu
            title: qsTr("Special effect")

            MenuItemGroup {
                id: specialEffectMenuGroup
                items: specialEffectMenu.items
            }

            MenuItem {
                enabled: Qt.platform.os === "windows"
                text: qsTr("DWM blur")
                checkable: true
                onTriggered: {
                    window.color = checked ? "transparent" : darkStyle.windowBackgroundColor
                    windowAgent.setWindowAttribute("dwm-blur", checked)
                }
            }

            MenuItem {
                enabled: Qt.platform.os === "windows"
                text: qsTr("Acrylic material")
                checkable: true
                onTriggered: {
                    window.color = checked ? "transparent" : darkStyle.windowBackgroundColor
                    windowAgent.setWindowAttribute("acrylic-material", checked)
                }
            }

            MenuItem {
                enabled: Qt.platform.os === "windows"
                text: qsTr("Mica")
                checkable: true
                onTriggered: {
                    window.color = checked ? "transparent" : darkStyle.windowBackgroundColor
                    windowAgent.setWindowAttribute("mica", checked)
                }
            }

            MenuItem {
                enabled: Qt.platform.os === "windows"
                text: qsTr("Mica Alt")
                checkable: true
                onTriggered: {
                    window.color = checked ? "transparent" : darkStyle.windowBackgroundColor
                    windowAgent.setWindowAttribute("mica-alt", checked)
                }
            }
        }
    }*/
}