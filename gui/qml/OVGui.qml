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

    function process_msg(text){

    }

    /* Helper Functions */
    function getEndPos() {
        var ratio = 1.0 - textflickable.visibleArea.heightRatio;
        var endPos = textflickable.contentHeight * ratio;
        return endPos;
    }

    /* Properties */
    property bool isConnected: false
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
            GridLayout {
                anchors.fill: parent
                flow: GridLayout.TopToBottom
                rows: 2
                CellBox {
                    // title: 'Range Controllers'
                    Layout.maximumWidth: 250
                    ColumnLayout {
                        anchors.fill: parent
                        RangeSlider {
                            id: rangeslider
                            first.value: 0.25;
                            // rangeslider.handle.radius: 10
                            second.value: 0.75;
                            Layout.fillWidth: true
                            ToolTip.visible: hovered
                            ToolTip.delay: 500
                            ToolTip.text: rangeslider.first.value.toFixed(2) + ' : ' + rangeslider.second.value.toFixed(2)
                        }
                        Dial {
                            id: dial
                            scale: 1.8
                            Layout.alignment: Qt.AlignHCenter
                            ToolTip {
                                parent: dial.handle
                                visible: dial.pressed
                                delay: 500
                                text: dial.value.toFixed(2)
                            }
                        }
                    }
                }
                CellBox {
                    // title: 'Spin Boxes'
                    Layout.maximumWidth: 250

                    ColumnLayout {
                        anchors.fill: parent
                        SpinBox {
                            value: 50;editable: true;Layout.fillWidth: true
                        }
                        SpinBox {
                            from: 0
                            to: items.length - 1
                            value: 1 // 'Medium'
                            property var items: ['Small', 'Medium', 'Large']
                            validator: RegExpValidator {
                                regExp: new RegExp('(Small|Medium|Large)', 'i')
                            }
                            textFromValue: function(value) {
                                return items[value];
                            }
                            valueFromText: function(text) {
                                for (var i = 0; i < items.length; ++i)
                                    if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
                                        return i
                                return sb.value
                            }
                            Layout.fillWidth: true
                        }
                        SpinBox {
                            id: doubleSpinbox
                            editable: true
                            from: 0
                            value: 110
                            to: 100 * 100
                            stepSize: 100
                            property int decimals: 2
                            property real realValue: value / 100
                            validator: DoubleValidator {
                                bottom: Math.min(doubleSpinbox.from, doubleSpinbox.to)
                                top: Math.max(doubleSpinbox.from, doubleSpinbox.to)
                            }
                            textFromValue: function(value, locale) {
                                return Number(value / 100).toLocaleString(locale, 'f', doubleSpinbox.decimals)
                            }
                            valueFromText: function(text, locale) {
                                return Number.fromLocaleString(locale, text) * 100
                            }
                            Layout.fillWidth: true
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
                        // onClicked: modalPopup.open()
                    }
                    Button {
                        text: 'E-STOP'
                        implicitWidth: 140
                        // onClicked: normalPopup.open()
                    }
                    Button {
                        text: 'Motion Calibrate'
                        implicitWidth: 230
                        // onClicked: normalPopup.open()
                    }
                }
            }
        }
    }
}
