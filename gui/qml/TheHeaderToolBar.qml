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
            }
            ToolSeparator {}
            ToolButton {
                text: 'Volume Mode'
            }
            ToolSeparator {}
            ToolButton {
                text: 'Breath Assist Mode'
            }
            ToolSeparator {}
            ToolButton {
                text: 'Manual Mode'
            }
            ToolSeparator {}
        }
    }