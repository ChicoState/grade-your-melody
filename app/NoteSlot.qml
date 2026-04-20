import QtQuick



Rectangle {
    id: root
    width: 50
    height: 25
    color: "#00000000"
    property int beat: -1
    property int row: -1 
    property int currentAcc: 0
    property int currentNoteLength: 1
    property bool selected: false
    property var wrongBeats: []
    property int gradeCount: 0
    property bool flipped: row >= 6   // above B4 middle line → stem down
    property int noteLen: 1           // tracks placed note type for transform
    
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
        transform: Scale {
            // 180° flip (both axes) = stem down, head facing right; whole notes never flip
            xScale: (root.flipped && root.noteLen !== 4) ? -1 : 1
            yScale: (root.flipped && root.noteLen !== 4) ? -1 : 1
            origin.x: placenote.width / 2
            origin.y: placenote.height / 2
        }
        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }
    // Ledger line — shown when a note is placed at or adjacent to a ledger line position
    Rectangle {
        id: ledgerLine
        width: 40
        height: 3
        color: "black"
        x: -1.3
        // rows 0 & 12: line through the note head (slot center)
        // rows 1 & 13: line one row below
        y: (row === 1 || row === 13)
            ? parent.height + (parent.height - height) / 2 + 2
            : (parent.height - height) / 2 + 2
        visible: selected && (row <= 1 || row >= 12)
    }

    AnimatedImage {
        id: wrongMark
        source: "images/redx.gif"
        width: 27
        height: 27
        visible: gradeCount > 0 && selected && !gridController.isBeatCorrect(beat)
        playing: visible
        cache: false
        speed: 2
        x: 5
        y: 3
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            if (root.state != "clicked") {
                // Don't show hover ghost if quarter note can't be placed here
                if (currentNoteLength === 2 && beat % 2 !== 0) return
                if (currentNoteLength === 3 && beat % 4 !== 0) return
                if (currentNoteLength === 4 && beat % 8 !== 0) return
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
                if (currentNoteLength === 3 && beat % 4 !== 0) return
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
                        root.noteLen = 1
                        placenote.source = "images/eighthnote.png"
                        placenote.x = root.flipped ? -35.5 : -19
                        placenote.y = root.flipped ? 1 : -62
                        placenote.width = 92
                        placenote.height = 91
                    } else if (len === 2) {
                        root.noteLen = 2
                        placenote.source = "images/quarternote.png"
                        placenote.x = root.flipped ? -32 : -28
                        placenote.y = root.flipped ? -2 : -62
                        placenote.width = 98
                        placenote.height = 96
                    } else if (len === 3) {
                        root.noteLen = 3
                        placenote.source = "images/halfnote.png"
                        placenote.x = root.flipped ? -29.5 : -29
                        placenote.y = root.flipped ? 3 : -63
                        placenote.width = 96
                        placenote.height = 94
                    } else if (len === 4) {
                        root.noteLen = 4
                        placenote.source = "images/wholenote.png"
                        placenote.x = 0
                        placenote.y = -3
                        placenote.width = 40
                        placenote.height = 38
                    }
                    if (acc === 1) {
                        placenote.source = "images/sharpnote.png"
                        placenote.x = -43
                        placenote.y = root.flipped ? -9 : -62
                        placenote.width = 98
                        placenote.height = 96
                    } else if (acc === -1) {
                        placenote.source = "images/flatnote.png"
                        placenote.x = -43
                        placenote.y = root.flipped ? -9 : -62
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
    onGradeCountChanged: {
        wrongMark.currentFrame = 0
        wrongMark.playing = (gradeCount > 0 && selected && !gridController.isBeatCorrect(beat))
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
