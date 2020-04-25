import QtQuick 2.13
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4


GroupBox {

    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.minimumWidth: 200

    background: Rectangle {
        y: parent.topPadding - parent.padding
        width: parent.width
        height: parent.height - parent.topPadding + parent.padding
        color: Material.accent
        radius: 2
    }

    property alias numValue: numValueLabel.text
    property alias numText: numTextLabel.text
    RowLayout {
        anchors.fill: parent
        Label {
            id: numTextLabel
            font.bold: true
            font.family: "Calibri"

            // color: Material['Grey']
        }
        Label {
            id: numValueLabel
            font.bold: true
            font.family: "Calibri"

            // color: Material['Grey']
        }
    }

    clip: true
}