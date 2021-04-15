import QtQuick 2.15
import QtQuick.Controls 2.15

ComboBox {
    id: control
    model: ["سلام", "سلام دو", "سلام سه"]
    font.family: regular_font.name
    font.weight: Font.DemiBold
    font.pixelSize: 17
    property real radius: 12
    property real popupMaxHeight: 150

    Behavior on height {
        NumberAnimation { duration: 150 }
    }

    delegate: Item {
        width: control.width
        height: item_delg.height
        ItemDelegate {
            id: item_delg
            width: parent.width
            highlighted: control.highlightedIndex === index

            onClicked: {
                control.currentIndex = index
                control.activated(index)
                pop.close()
            }

            contentItem: Text {
                text: modelData[textRole] === undefined ? modelData : modelData[textRole]
                color: "#FFFFFF"
                font: control.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }

        // separator between items
        Rectangle {
            visible: index !== control.count - 1
            width: parent.width
            height: 1
            y: parent.height
            color: "#159E84"
            opacity: 0.5
        }
    }

    indicator: Canvas {
        id: canvas
        x: width + control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        contextType: "2d"

        Connections {
            target: pop
            function onAboutToShow() { rotate.from= 0; rotate.to = 180; rotate.start() }
            function onAboutToHide() { rotate.from= 180; rotate.to = 360; rotate.start() }
        }

        Component.onCompleted: requestPaint()
        onPaint: {
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = "#121212";
//                context.strokeStyle = "#121212";
            context.fill();
        }
        RotationAnimation {
            id: rotate
            target: canvas
            duration: 150
        }
    }

    contentItem: Text {
        rightPadding: 0
        leftPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
//            color: control.pressed ? "#17a81a" : "#21be2b"
        color: "#262125"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
//            border.width: control.visualFocus ? 2 : 1
        color: "#19B99A"
        radius: control.radius

    }

    popup: Popup {
        id: pop
        y: control.height - 15
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight <= control.popupMaxHeight ? contentHeight + 5 : control.popupMaxHeight

            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            boundsBehavior: ListView.StopAtBounds

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            color: "#116A59"
            radius: control.radius
        }

        onAboutToShow: {
            control.height += 10
        }
        onAboutToHide: {
            control.height -= 10
        }

    }
}
