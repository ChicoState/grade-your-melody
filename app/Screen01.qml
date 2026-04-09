


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
    
    property int currentScore: 0
    property var wrongBeats: []
    property bool hasGraded: false



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
    spacing: 12
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.leftMargin: 30
    anchors.topMargin: 30
    
    Text {
        text: hasGraded ? ("Score: " + currentScore + " / 16") : "Score: --"
        font.pixelSize: 24
        color: "black"
    }
    
    Row {
        spacing: 10

        Button { text: "Flat"; onClicked: rectangle.currentAcc = -1 }
        Button { text: "Natural"; onClicked: rectangle.currentAcc = 0 }
        Button { text: "Sharp"; onClicked: rectangle.currentAcc = 1 }
    }

Text{ 
    id: benchmarkText
    text: "place"
}

Connections {
    target: gridController

    function onBenchmarkFinished(result) {
    benchmarkText.text = result
    }
}

Button {
    text: "run a BILLION "
    onClicked: {
        gridController.runMillion()
    }
}

Button {
        text: "Grade"
        onClicked: {
        currentScore = gridController.score()
        wrongBeats = gridController.incorrectBeats()
        hasGraded = true 


            console.log("Score:", gridController.score(), "/16")
            console.log("Incorrect beats:", gridController.incorrectBeats())

        }
    }
}




}


