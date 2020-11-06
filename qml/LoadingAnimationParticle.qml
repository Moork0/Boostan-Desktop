import QtQuick 2.15

Item {
    id: root
    property alias radius: container.radius
    property alias color: container.color
    property alias particleSize: particle.width
    property alias particleColor: particle.color

    property int particleNumber: 3

    Rectangle {
        id: container
        anchors.fill: parent
        color: "#19B99A"
        radius: 5
        Repeater {
            id: repeater
            model: root.particleNumber

            Rectangle {
                id: particle
                width: 20
                height: width
                radius: width / 2
                anchors.verticalCenter: parent.verticalCenter
                property alias run: animation.running

                SequentialAnimation {
                    id: animation
                    loops: Animation.Infinite
                    NumberAnimation {
                        from: 0
                        to: 10
                        duration: 500
                        easing.type: Easing.OutInQuart
                    }

                    ParallelAnimation {
                        NumberAnimation {
                            target: particle
                            property: "x"
                            from: 0
                            to: pp.width - ((particle.width + (particle.width / 2)) * index)
                            duration: 1500
                            easing.type: Easing.OutInQuart
                        }

                        NumberAnimation {
                            target: particle
                            property: "opacity"
                            from: 1.0
                            to: 0.0
                            duration: 1500
                            easing.type: Easing.InQuint
                        }
                    }

                    NumberAnimation {
                        from: 0
                        to: 10
                        duration: 500
                    }
                }
            }
            Component.onCompleted: timer.start()
        }

        Timer {
            id: timer
            property int index: 0
            interval: 500
            repeat: true
            onTriggered: {
                repeater.itemAt(index).run = true
                ++index
                if (index == root.particleNumber) timer.stop()
            }
        }

    }

}

