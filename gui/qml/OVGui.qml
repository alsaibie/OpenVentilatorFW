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

        current_pos = textflickable.contentY;
        end_position = getEndPos()

        if (current_pos == end_position) {
            textflickable.contentY = end_position
        } else {
            textflickable.contentY = current_pos
        }
    }

    /* Chart Update */
    function updatePlot(x, y, seriesName) {
        /* trampoline */
        chartsBox.updatePlot(x, y, seriesName)
    }

    /* Update Value Row */
    function updateValuesRow(value, valueName){
        /* trampoline */
        valueRow.updateValuesRow(value, valueName)
    }

    /* Flush plots */
    function flushPlots(){
        chartsBox.flushPlots()
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

    property var opModes: {"0": "Manual", "1": "Pressure", "2" : "Pressure Assist", "3" : "Volume"};
    property var opState: {"0": "Disarmed", "1" : "Ready", "2" : "Running", "3" : "Running Warning", "4" : "Calibrating"}

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
            // CellBox {
            //     Layout.maximumHeight: 50
            RowLayout {
                id: valueRow
                function updateValuesRow(value, which) {
                    if (which == "uptime") {
                        uptimeBox.numValue = value.toFixed(0) + 's'
                    } else if (which == "minPressure") {
                        minPressureBox.numValue = value.toFixed(2)
                    } else if (which == "maxPressure") {
                        maxPressureBox.numValue = value.toFixed(2)
                    } else if (which == "volume") {
                        volumeBox.numValue = value.toFixed(2)
                    } else if (which == "opMode") {
                        operatingModeBox.numValue = opModes[value.toFixed(0)]
                    } else if (which == "opState") {
                        operatingStateBox.numValue = opState[value.toFixed(0)]
                    }
                }
                Layout.maximumHeight: 50
                // anchors.top: parent.top
                NumberBox {
                    id: uptimeBox
                    numText: "Uptime"
                    numValue: Number(12).toFixed(0)
                }
                NumberBox {
                    id: minPressureBox
                    numText: "Minimum Pressure"
                    numValue: Number(12).toFixed(0)
                }
                NumberBox {
                    id: maxPressureBox
                    numText: "Maximum Pressure"
                    numValue: Number(12).toFixed(0)
                }
                NumberBox {
                    id: volumeBox
                    numText: "Volume"
                    numValue: Number(12).toFixed(0)
                }
                NumberBox {
                    id: operatingModeBox
                    numText: "Operating Mode"
                    numValue: Number(12).toFixed(0)
                }
                NumberBox {
                    id: operatingStateBox
                    numText: "Operating State"
                    numValue: Number(12).toFixed(0)
                }
            }
            // }
            ColumnLayout {
                // anchors.fill: parent
                RowLayout {
                    // anchors.fill: parent
                    CellBox {
                        Layout.maximumWidth: 250
                        ColumnLayout {
                            anchors.fill: parent
                            Label {
                                text: "Pressure Range\n" + pressureSlider.first.value.toFixed(0) + 'mmH2O' + ' : ' + pressureSlider.second.value.toFixed(0) + 'mmH2O'
                                // Layout.fillWidth: true
                                // anchors.bottom: pressureSlider.top
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
                                text: "PositionTest: " + sliderOne.value.toFixed(0)
                                Layout.fillWidth: true
                                // anchors.bottom: sliderOne.top
                                horizontalAlignment: Text.AlignHCenter
                                color: Material.accent
                            }
                            Slider {
                                id: sliderOne
                                from: -800
                                to: 800
                                stepSize: 1
                                value: 0
                                Layout.fillWidth: true
                                ToolTip.visible: hovered
                                ToolTip.delay: 500
                                ToolTip.text: sliderOne.value.toFixed(2)
                                onValueChanged: pygui.sendFloatCommand("PosSp", sliderOne.value.toFixed(0))
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
                                }

                                onValueChanged: pygui.sendFloatCommand("FreqSp", frequencyDial.value.toFixed(0))
                            }
                        }
                    }
                    TheChartsCell {
                        id: chartsBox // Need to expose imported parent object in order to reference it
                    }
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
                            onClicked: pygui.sendBinaryCommand("ESTOP", true)
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
}
