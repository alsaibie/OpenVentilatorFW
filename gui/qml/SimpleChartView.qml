import QtCharts 2.2
import QtQuick.Layouts 1.11
ChartView {
    titleColor: '#f2faf9'
    titleFont: Qt.font({
        pointSize: 11,
        bold: true
    })
    Layout.fillWidth: true
    Layout.fillHeight: true
    legend.visible: false
    margins.top: 0
    margins.bottom: 0
    margins.left: 0
    margins.right: 0
    backgroundColor: '#303030'
    antialiasing: true
    Layout.minimumWidth: 200
    clip: true
}