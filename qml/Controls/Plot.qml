/*
    * Plot component for drawing a Bar plot
    * this component will be rewritten.
*/

// TODO: rewrite this component
import QtQuick 2.15
import QtCharts 2.15

ChartView {
    id: root
    title: "معدل ترمی"
    titleColor: "#FFFFFF"
    titleFont.family: fontFamily
    titleFont.weight: Font.Black
    antialiasing: true
    legend.visible: false
    locale: Qt.locale("fa_IR")
    localizeNumbers: true
    animationDuration: 500
    animationOptions: ChartView.SeriesAnimations
    backgroundColor: "transparent"

    property string fontFamily: "Arial"
    property alias xAxis: axis_x.categories
    property alias yAxis: bar_set.values

    BarCategoryAxis {
        id: axis_x
        color: "#FFFFFF"
        gridVisible: false
        labelsColor: "#FFFFFF"
        labelsFont.family: root.fontFamily
        labelsFont.weight: Font.Black
        lineVisible: false
    }

    ValueAxis {
        id: axis_y
        color: "#FFFFFF"
        gridLineColor: "#FFFFFF"
        labelsColor: "#FFFFFF"
        labelsFont.family: root.fontFamily
        labelsFont.weight: Font.Black
        max: 20
        min: 10
//        gridVisible: false
    }

    BarSeries {
        id: bar_serie
        axisX: axis_x
        axisY: axis_y
        barWidth: 0.35
        labelsVisible: true
        labelsPosition: AbstractBarSeries.LabelsInsideEnd
        BarSet {
            id: bar_set
            borderColor: "transparent"
            borderWidth: 0
            color: "#19B99A"
            labelColor: "#FFFFFF"
            labelFont.family: root.fontFamily
        }
    }
}
