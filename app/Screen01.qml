


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
    property int currentAcc: 0 // -1 flat, +1 sharp 
   onCurrentAccChanged: console.log("currentAcc now", currentAcc) 
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
                row: Math.floor(index / 4)
                currentAcc: rectangle.currentAcc
            }
        }
        // Measure 2
        Repeater {
            model: 36
            NoteSlot {
                x: 276 + (324 + 50) + (index % 4) * 81 //Move one full measure (324) plus offset of 50
                y: 616 - Math.floor(index / 4) * 25
                beat: 4 + (index % 4) //Shift occupied beats to start at beat 4
                row: Math.floor(index / 4)
                currentAcc: rectangle.currentAcc
            }
        }
        // Measure 3
        Repeater {
            model: 36
            NoteSlot {
                x: 276 + (648 + 125) + (index % 4) * 81 //Move one full measure (324) plus offset of 50
                y: 616 - Math.floor(index / 4) * 25
                beat: 8 + (index % 4) //Shift occupied beats to start at beat 4
                row: Math.floor(index / 4)
                currentAcc: rectangle.currentAcc
            }
        }
        // Measure 4
        Repeater {
            model: 36
            NoteSlot {
                x: 276 + (972 + 200) + (index % 4) * 81 //Move one full measure (324) plus offset of 50
                y: 616 - Math.floor(index / 4) * 25
                beat: 12 + (index % 4) //Shift occupied beats to start at beat 4
                row: Math.floor(index / 4)
                currentAcc: rectangle.currentAcc
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

Column {
    spacing: 20
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: staffLines2.bottom
    anchors.topMargin: -20

    Row {
        spacing: 10

        Image {
            source: "images/flatbutton.png"
            fillMode: Image.PreserveAspectFit
            height: 40
            opacity: rectangle.currentAcc === -1 ? 0.6 : 1.0
            MouseArea {
                anchors.fill: parent
                onClicked: rectangle.currentAcc = -1
            }
        }
        Image {
            source: "images/naturalbutton.png"
            fillMode: Image.PreserveAspectFit
            height: 40
            opacity: rectangle.currentAcc === 0 ? 0.6 : 1.0
            MouseArea {
                anchors.fill: parent
                onClicked: rectangle.currentAcc = 0
            }
        }
        Image {
            source: "images/sharpbutton.png"
            fillMode: Image.PreserveAspectFit
            height: 40
            opacity: rectangle.currentAcc === 1 ? 0.6 : 1.0
            MouseArea {
                anchors.fill: parent
                onClicked: rectangle.currentAcc = 1
            }
        }
    }

    Image {
        source: "images/gradebutton.png"
        fillMode: Image.PreserveAspectFit
        height: 40
        anchors.horizontalCenter: parent.horizontalCenter
        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("Score:", gridController.score(), "/16")
                console.log("Wrong beats:", gridController.incorrectBeats())
            }
        }
    }
}




}


