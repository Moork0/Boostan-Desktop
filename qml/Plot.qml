import QtQuick 2.15
import QtCharts 2.15

ChartView {
    title: "معدل ترمی"
    titleColor: "#FFFFFF"
    titleFont.family: "Sahel"
    antialiasing: true
    legend.visible: false
    locale: Qt.locale("fa_IR")
    localizeNumbers: true
    animationDuration: 500
    animationOptions: ChartView.SeriesAnimations
    backgroundColor: "transparent"

    property alias xAxis: axis_x.categories
    property alias yAxis: bar_set.values

    BarCategoryAxis {
        id: axis_x
        categories: ["۹۷۱", "۹۷۲", "۹۸۱"]
        color: "#FFFFFF"
        gridVisible: false
        labelsColor: "#FFFFFF"
        labelsFont.family: "Sahel"
        lineVisible: false
    }

    ValueAxis {
        id: axis_y
        color: "#FFFFFF"
        gridLineColor: "#FFFFFF"
        labelsColor: "#FFFFFF"
        labelsFont.family: "Sahel"
        labelsFont.bold: true
        max: 20
        min: 10
//        gridVisible: false
    }

    BarSeries {
        id: bar_serie
        axisX: axis_x
        axisY: axis_y
        barWidth: 0.2
        labelsVisible: true
        labelsPosition: AbstractBarSeries.LabelsInsideEnd
        BarSet {
            id: bar_set
            values: ["15.2", "16.4", "17"]
            borderColor: "transparent"
            borderWidth: 0
            color: "#19B99A"
            labelColor: "#FFFFFF"
            labelFont.family: "Sahel"
        }
    }
}
