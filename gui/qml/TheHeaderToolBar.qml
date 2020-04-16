import QtQuick 2.13
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4

ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                icon.source: '../images/baseline-menu-24px.svg'
                onClicked: sideNav.open()
            }
            Label {
                text: 'Open Ventilator'
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            ToolButton {
                text: 'Pressure Mode'
                onClicked: pygui.sendBinaryCommand("PressureMode", true)
            }
            ToolSeparator {}
            ToolButton {
                text: 'Volume Mode'
                onClicked: pygui.sendBinaryCommand("VolumeMode", true)
            }
            ToolSeparator {}
            ToolButton {
                text: 'Breath Assist Mode'
                onClicked: pygui.sendBinaryCommand("BreathAssistMode", true)
            }
            ToolSeparator {}
            ToolButton {
                text: 'Manual Mode'
                onClicked: pygui.sendBinaryCommand("ManualMode", true)
            }
            ToolSeparator {}
        }
    }