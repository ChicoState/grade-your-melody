import QtQuick



Rectangle {
    id: root
    width: 45
    height: 35
    color: "#00000000"
    property int beat: -1
    property int row: -1 
    property int currentAcc: 0
    property bool selected: false
    
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
            if(!gridController) return 
            if (selected) {
                gridController.clearBeat(beat)
            } else  {
                console.log("clicked beat", beat, "row", row, "acc", currentAcc)
                gridController.setNote(beat, row, currentAcc)
            }
        }
    }
    // When any slot changes in this beat, refresh this slot's visuals
    Connections {
        target: gridController
        function onBeatChanged(changedBeat) {
            if (changedBeat === beat && gridController) {
                selected = gridController.hasNote(beat, row)
                if (selected) {
                    var acc = gridController.accidentalForBeat(beat)
                    if (acc === 1)
                        placenote.source = "images/sharpnote.png"
                    else if (acc === -1)
                        placenote.source = Qt.resolvedUrl("images/quarternote.png")
                    else
                        placenote.source = "images/quarternote.png"
                }
            }
        }
    }
 
    // Keep state synced
    Component.onCompleted: {
        if(gridController){
            selected = gridController.hasNote(beat,row)
        }
        }
    onSelectedChanged: root.state = root.selected ? "clicked" : ""

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
