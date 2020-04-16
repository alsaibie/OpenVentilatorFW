// Remember to run:  pyrcc5 -o resources.py resources.qrc

import QtQuick 2.13
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtCharts 2.2

ApplicationWindow {
    /* Signals */
    signal connectDevice;
    signal disconnectDevice;

    /* Slot Functions */
    function connectionStatus(state) {
        isConnected = state
        connectButton.text = isConnected ? "CONNECTED" : "CONNECT"
    }

    function isReadyStatus(state) {
        isReady = state
        startStopSwitch.enabled = isReady ? true : false
    }
    function operationState(state) {

    }

    function incomingLine(text) {
        var current_pos, end_position;

        if (dispIncoming) {
            textBox.append(text)
        }

        process_msg(text)

        current_pos = textflickable.contentY;
        end_position = getEndPos()

        if (current_pos == end_position) {
            textflickable.contentY = end_position
        } else {
            textflickable.contentY = current_pos
        }
    }

    /* Data Processing Functions */

    function process_msg(text) {

    }

    /* Helper Functions */
    function getEndPos() {
        var ratio = 1.0 - textflickable.visibleArea.heightRatio;
        var endPos = textflickable.contentHeight * ratio;
        return endPos;
    }

    /* Properties */
    property bool isConnected: false
    property bool isPaused: false
    property bool isHoming: false
    property bool isReady: false
    property bool dispIncoming: false


    visible: true
    minimumHeight: 1000
    minimumWidth: 1880
    font.family: "Calibri"
    font.pointSize: 11
    Material.theme: Material['Dark']
    Material.accent: Material['Blue']
    Material.primary: Material['Blue']

    header: TheHeaderToolBar {}

    Drawer {
        id: sideNav
        width: 800
        height: parent.height
        Flickable {
            id: textflickable
            flickableDirection: Flickable.VerticalFlick
            anchors.fill: parent
            TextArea.flickable: TextArea {
                id: textBox
                placeholderText: 'Multi-line text editor...'
                wrapMode: TextArea.Wrap
                selectByMouse: true
                persistentSelection: true
                leftPadding: 6
                rightPadding: 6
                topPadding: 6
                bottomPadding: 6
            }
            ScrollBar.vertical: ScrollBar {
                id: textScrollBar
            }
        }
    }
    Pane {
        padding: 10
        anchors.fill: parent
        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                anchors.fill: parent
                // flow: GridLayout.TopToBottom
                // rows: 2
                CellBox {
                    // title: 'Range Controllers'
                    // rowSpan : 2
                    Layout.maximumWidth: 250
                    ColumnLayout {
                        anchors.fill: parent
                        Label {
                            text: "Pressure Range\n" + pressureSlider.first.value.toFixed(0) + 'mmH2O' + ' : ' + pressureSlider.second.value.toFixed(0) + 'mmH2O'
                            Layout.fillWidth: true
                            anchors.bottom: pressureSlider.top
                            horizontalAlignment: Text.AlignHCenter
                            color: Material.accent
                        }
                        RangeSlider {
                            id: pressureSlider
                            from: 100
                            to: 700
                            stepSize: 20
                            first.value: 200;
                            second.value: 600;
                            Layout.fillWidth: true
                            ToolTip.visible: hovered
                            ToolTip.delay: 500
                            ToolTip.text: pressureSlider.first.value.toFixed(2) + ' : ' + pressureSlider.second.value.toFixed(2)
                        }
                        Label {
                            text: "Frequency"
                            Layout.fillWidth: true
                            anchors.centerIn: frequencyDial.center
                            horizontalAlignment: Text.AlignHCenter
                            color: Material.accent
                        }
                        Dial {
                            id: frequencyDial
                            scale: 1.8
                            stepSize: 1
                            snapMode: Dial.SnapAlways
                            from: 20
                            to: 100
                            Layout.alignment: Qt.AlignHCenter
                            contentItem: Text {
                                text: frequencyDial.value.toFixed(0) + 'Hz'
                                color: Material.accent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                opacity: 0.7
                                // elide: Text.ElideRight
                            }
                            // ToolTip {
                            //     parent: frequencyDial.handle
                            //     visible: frequencyDial.pressed
                            //     // visible: true
                            //     delay: 500
                            //     text: frequencyDial.value.toFixed(2)
                            onValueChanged: pygui.sendFloatCommand("FrequencySP", frequencyDial.value)
                        }
                    }
                }
                TheChartsCell {}
            }
            CellBox {
                // title: 'Buttons'
                Layout.maximumHeight: 80
                // width: mainWindow.width
                // Layout.columnSpan: 2
                RowLayout {
                    anchors.fill: parent
                    Switch {
                        id: startStopSwitch
                        checked: false
                        text: startStopSwitch.checked ? "START" : "STOP"
                        Layout.fillWidth: true
                        enabled: false
                    }
                    CheckBox {
                        id: dispLogCheckBox
                        text: 'Display Serial Messages'
                        checkable: true
                        onToggled: function() {
                            dispIncoming = dispLogCheckBox.checked ? true : false;
                        }
                    }
                    Button {
                        id: connectButton
                        text: 'CONNECT'
                        implicitWidth: 149
                        onClicked: isConnected ? disconnectDevice() : connectDevice()
                        highlighted: isConnected ? true : false

                    }
                    Button {
                        text: 'PAUSE'
                        implicitWidth: 140
                        onClicked: pygui.sendBinaryCommand("Pause", true)

                    }
                    Button {
                        text: 'E-STOP'
                        implicitWidth: 140
                        onClicked: pygui.sendBinaryCommand("E-STOP", true)
                    }
                    Button {
                        text: 'Motion Calibrate'
                        implicitWidth: 230
                        onClicked: pygui.sendBinaryCommand("MCalibrate", true)
                    }
                }
            }
        }
    }
}
