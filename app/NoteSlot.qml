import QtQuick



Rectangle {
    id: root
    width: 45
    height: 35
    color: "#00000000"
    property int beat: -1
    property int row: -1 
    property int currentAcc: 0
    property int currentNoteLength: 1
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
            if (root.state != "clicked") {
                // Don't show hover ghost if quarter note can't be placed here
                if (currentNoteLength === 2 && beat % 2 !== 0) return
                root.state = "hovered"
            }
        }
        onExited: if (root.state != "clicked") root.state = ""
        onPressed: {
            if(!gridController) return
            if (selected) {
                gridController.clearBeat(beat)
            } else  {
                // Quarter notes (length 2) can only go on even beats (main beats)
                if (currentNoteLength === 2 && beat % 2 !== 0) return
                console.log("clicked beat", beat, "row", row, "acc", currentAcc, "len", currentNoteLength)
                gridController.setNote(beat, row, currentAcc, currentNoteLength)
            }
        }
    }
    // When any slot changes in this beat, refresh this slot's visuals
    Connections {
        target: gridController
        function onBeatChanged(changedBeat) {
            if (changedBeat === beat && gridController) {
                var len = gridController.noteLengthForBeat(beat)
                var hasNoteHere = gridController.hasNote(beat, row)

                // Only show on note start beats (len > 0), not continuations
                selected = hasNoteHere && len > 0
                if (selected) {
                    var acc = gridController.accidentalForBeat(beat)

                    if (len === 1) {
                        placenote.source = "images/eighthnote.png"
                        placenote.x = -20
                        placenote.width = 92
                        placenote.height = 91
                    } else if (acc === 1) {
                        placenote.source = "images/sharpnote.png"
                        placenote.x = -43
                        placenote.width = 98
                        placenote.height = 96
                    } else if (acc === -1) {
                        placenote.source = "images/flatnote.png"
                        placenote.x = -43
                        placenote.width = 98
                        placenote.height = 96
                    } else {
                        placenote.source = "images/quarternote.png"
                        placenote.x = -28
                        placenote.width = 98
                        placenote.height = 96
                    }
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
