


import QtQuick
import QtQuick.Controls
import GradeYourMelodyUI

Rectangle {
    id: rectangle
    width: 1920
    height: 1080
    color: "#EAEAEA"
    clip: true
    //16 beats
    property var occupiedBeats: [false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false]

    Image {
        id: staffLines2
        x: 53
        y: 403
        width: 1814
        height: 275
        source: "images/staff lines 2.png"
        fillMode: Image.PreserveAspectFit
    }
    Item {
        x: 0
        y: 0
        width: 1920
        height: 1080

        // Measure 1
        Repeater {
            model: 36
            NoteSlot {
                x: 276 + (index % 4) * 81
                y: 616 - Math.floor(index / 4) * 25
                beat: index % 4
            }
        }
        // Measure 2
        Repeater {
            model: 36
            NoteSlot {
                x: 276 + (324 + 50) + (index % 4) * 81 //Move one full measure (324) plus offset of 50
                y: 616 - Math.floor(index / 4) * 25
                beat: 4 + (index % 4) //Shift occupied beats to start at beat 4
            }
        }
        // Measure 3
        Repeater {
            model: 36
            NoteSlot {
                x: 276 + (648 + 125) + (index % 4) * 81 //Move one full measure (324) plus offset of 50
                y: 616 - Math.floor(index / 4) * 25
                beat: 8 + (index % 4) //Shift occupied beats to start at beat 4
            }
        }
        // Measure 4
        Repeater {
            model: 36
            NoteSlot {
                x: 276 + (972 + 200) + (index % 4) * 81 //Move one full measure (324) plus offset of 50
                y: 616 - Math.floor(index / 4) * 25
                beat: 12 + (index % 4) //Shift occupied beats to start at beat 4
            }
        }
    }

    Image {
        id: title
        x: 447
        y: 312
        width: 1027
        height: 72
        source: "images/title.png"
        fillMode: Image.PreserveAspectFit
    }
}



