import QtQuick 2.13
import QtQuick.Layouts 1.11
import QtCharts 2.2

CellBox {
    /* Can expose child id's here, but instead use trampoline function */
    // property SplineSeries flow1: series1Flow ... etc

    Layout.minimumWidth: 700
    id: chartsBox

    function updatePlot(x, y, seriesName) {
        if (seriesName == "Flow1") {
            updatePlotSeries(x, y, series1Flow, true)
        } else if (seriesName == "Flow2") {
            updatePlotSeries(x, y, series2Flow, false)
        } else if (seriesName == "Volume") {
            updatePlotSeries(x, y, seriesVolume, true)
        } else if (seriesName == "Pressure") {
            updatePlotSeries(x, y, seriesPressure, true)
        } else if (seriesName == "PEEP") {
            updatePlotSeries(x, y, seriesPEEP, true)
        }
    }

    function updatePlotSeries(x, y, series, updateXAxis) {
        series.append(x, y)
        series.axisX.max = x + 2
        series.axisX.min = x - 20
        if (updateXAxis) {
            if (series.amountOfData > series.length_data) {
                series.remove(0)
            } else {
                series.amountOfData++
            }
        }
    }

    function flushPlots() {
        console.log("flush")
        flushPlot(series1Flow)
        flushPlot(seriesVolume)
    }

    function flushPlot(series) {
        if (series.amountOfData > 1) {
            // series.remove(series.amountOfData)
            //TODO: Fix it, it crashes when I attempt to flush
        }
    }

    GridLayout {
        id: gL
        flow: GridLayout.TopToBottom
        anchors.fill: parent
        rows: 2
        SimpleChartView {
            antialiasing: true
            id: pressureChartView
            title: 'Pressure'
            SplineSeries {
                id: seriesPressure
                name: 'Pressure'
                property int length_data: 200
                property int amountOfData: 0
                width: 8
                color: '#2196f3'
                axisY: YAxis {
                    min: -4
                    max: 4
                }
                axisX: TimeAxis {
                    min: 0
                    max: seriesPressure.length_data
                }
            }
        }
        SimpleChartView {
            antialiasing: true
            id: flowChartView
            title: 'Flow'
            SplineSeries {
                name: 'Flow Rate 1'
                property int length_data: 200
                property int amountOfData: 0
                id: series1Flow
                width: 8
                color: '#2196f3'
                axisY: YAxis {
                    min: -0
                    max: 12000
                }
                axisX: TimeAxis {
                    min: 0
                    max: series1Flow.length_data
                }
            }
            SplineSeries {
                name: 'Flow Rate 2'
                id: series2Flow
                property int length_data: 200
                property int amountOfData: 0
                width: 8
                color: '#90caf9'
            }
        }
        SimpleChartView {
            antialiasing: true
            id: volumeChartView
            title: 'Volume'
            SplineSeries {
                id: seriesVolume
                name: 'Volume'
                property int length_data: 200
                property int amountOfData: 0
                width: 8
                color: '#2196f3'
                axisY: YAxis {
                    min: 0
                    max: 12000
                }
                axisX: TimeAxis {
                    min: 0
                    max: seriesVolume.length_data
                }
            }
        }
        SimpleChartView {
            antialiasing: true
            id: peepChartView
            title: 'PEEP'
            SplineSeries {
                name: 'PEEP'
                id: seriesPEEP
                property int length_data: 200
                property int amountOfData: 0
                width: 8
                color: '#2196f3'
                axisY: YAxis {
                    min: 0
                    max: 12000
                }
                axisX: TimeAxis {
                    min: 0
                    max: seriesPEEP.length_data
                }
            }
        }
    }
}