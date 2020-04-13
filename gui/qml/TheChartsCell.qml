import QtQuick 2.13
import QtQuick.Layouts 1.11
import QtCharts 2.2

CellBox {
    Layout.rowSpan: 2
    Layout.minimumWidth: 700
    id: chartsBox

    GridLayout {
        flow: GridLayout.TopToBottom
        anchors.fill: parent
        rows: 2
        SimpleChartView {
            antialiasing: true
            id: pressureChartView
            title: 'Pressure'
            Timer {
                property int pressure_amountOfData: 0
                interval: 200
                running: true
                repeat: true
                onTriggered: {
                    pygui.update_pressuredata_series(seriesPressure, seriesPressure.length_data)
                    if (pressure_amountOfData > seriesVolume.length_data) {
                        pressure_axisX.max++
                        pressure_axisX.min++
                    } else {
                        pressure_amountOfData++
                    }
                }
            }
            SplineSeries {
                id: seriesPressure
                property int length_data: 50
                width: 8
                color: '#2196f3'
                axisY: YAxis {
                    id: pressure_axisY
                    min: -2.4
                    max: 2.4
                }
                axisX: TimeAxis {
                    id: pressure_axisX
                    min: 0
                    max: seriesPressure.length_data
                }
            }
        }
        SimpleChartView {
            antialiasing: true
            id: flowChartView
            title: 'Flow'
            Timer {
                property int flow_amountOfData: 0
                interval: 200
                running: true
                repeat: true
                onTriggered: {
                    pygui.update_flowdata_series(series1Flow, series2Flow, series1Flow.length_data)
                    if (flow_amountOfData > series1Flow.length_data) {
                        flow_axisX.max++
                        flow_axisX.min++
                    } else {
                        flow_amountOfData++
                    }
                }
            }
            SplineSeries {
                // name: 'Flow Rate'
                // title: 'Flow Rate'
                id: series1Flow
                property int length_data: 50
                width: 8
                color: '#2196f3'
                axisY: YAxis {
                    id: flow_axisY
                    min: -2.4
                    max: 2.4
                }
                axisX: TimeAxis {
                    id: flow_axisX
                    min: 0
                    max: series1Flow.length_data
                }
            }
            SplineSeries {
                // name: 'Flow Rate'
                // title: 'Flow Rate'
                id: series2Flow
                property int length_data: 50
                width: 8
                color: '#90caf9'
            }
        }
        SimpleChartView {
            antialiasing: true
            id: volumeChartView
            title: 'Volume'
            Timer {
                property int vol_amountOfData: 0
                interval: 200
                running: true
                repeat: true
                onTriggered: {
                    pygui.update_volumedata_series(seriesVolume, seriesVolume.length_data)
                    if (vol_amountOfData > seriesVolume.length_data) {
                        volume_axisX.max++
                        volume_axisX.min++
                    } else {
                        vol_amountOfData++
                    }
                }
            }
            SplineSeries {
                id: seriesVolume
                property int length_data: 50
                width: 8
                color: '#2196f3'
                axisY: YAxis {
                    id: volume_axisY
                    min: -2.4
                    max: 2.4
                }
                axisX: TimeAxis {
                    id: volume_axisX
                    min: 0
                    max: seriesVolume.length_data
                }
            }
        }
        SimpleChartView {
            antialiasing: true
            id: peepChartView
            title: 'PEEP'
            Timer {
                property int peep_amountOfData: 0
                interval: 200
                running: true
                repeat: true
                onTriggered: {
                    pygui.update_peepdata_series(seriesPEEP, seriesPEEP.length_data)
                    if (peep_amountOfData > seriesPEEP.length_data) {
                        peep_axisX.max++
                        peep_axisX.min++
                    } else {
                        peep_amountOfData++
                    }
                }
            }
            SplineSeries {
                // name: 'Flow Rate'
                // title: 'Flow Rate'
                id: seriesPEEP
                property int length_data: 50
                width: 8
                color: '#2196f3'
                axisY: YAxis {
                    id: peep_axisY
                    min: -2.4
                    max: 2.4
                }
                axisX: TimeAxis {
                    id: peep_axisX
                    min: 0
                    max: seriesPEEP.length_data
                }
            }
        }
    }
}