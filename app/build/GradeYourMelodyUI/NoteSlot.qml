import QtQuick



Rectangle {
    id: root
    width: 45
    height: 35
    color: "#00000000"
    property int beat: 0

    Image {
        id: hovernote
        x: 4
        y: 3
        width: 31
        height: 27
        visible: false
        source: "images/note transparent cut.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: placenote
        x: -28
        y: -62
        width: 98
        height: 96
        visible: false
        source: "images/quarternote.png"
        fillMode: Image.PreserveAspectFit
        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            if (root.state != "clicked" && !rectangle.occupiedBeats[beat]) {
                root.state = "hovered"
            }
        }
        onExited: if (root.state != "clicked") root.state = ""
        onPressed: {
            if (root.state == "clicked") {
                root.state = "hovered"
                rectangle.occupiedBeats[beat] = false
            } else if (!rectangle.occupiedBeats[beat]) {
                root.state = "clicked"
                rectangle.occupiedBeats[beat] = true
            }
        }
    }

    states: [
        State {
            name: "hovered"
            PropertyChanges { target: hovernote; visible: true }
        },
        State {
            name: "clicked"
            PropertyChanges { target: placenote; visible: true; opacity: 1 }
        }
    ]
}
