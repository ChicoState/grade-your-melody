


import QtQuick
import QtQuick.Controls
import GradeYourMelodyUI

Rectangle {
    id: rectangle
    width: 1920
    height: 1100
    color: "#EAEAEA"
    clip: true
    //32 beats
    property var occupiedBeats: [false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false]
    property int currentAcc: 0 // -1 flat, +1 sharp 
    property int currentNoteLength: 2 // 1 = eighth note, 2 = quarter note
    property int currentScore: 0
    property var wrongBeats: []
    property int gradeCount: 0 // increments each time grade is clicked
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

        // Measure 1 (8 slots × 9 rows = 72)
        Repeater {
            model: 72
            NoteSlot {
                wrongBeats: rectangle.wrongBeats
                gradeCount: rectangle.gradeCount
                x: 276 + (index % 8) * 40
                y: 616 - Math.floor(index / 8) * 25
                beat: index % 8
                row: Math.floor(index / 8)
                currentAcc: rectangle.currentAcc
                currentNoteLength: rectangle.currentNoteLength
            }
        }
        // Measure 2
        Repeater {
            model: 72
            NoteSlot {
                wrongBeats: rectangle.wrongBeats
                gradeCount: rectangle.gradeCount
                x: 276 + (324 + 50) + (index % 8) * 40
                y: 616 - Math.floor(index / 8) * 25
                beat: 8 + (index % 8)
                row: Math.floor(index / 8)
                currentAcc: rectangle.currentAcc
                currentNoteLength: rectangle.currentNoteLength
            }
        }
        // Measure 3
        Repeater {
            model: 72
            NoteSlot {
                wrongBeats: rectangle.wrongBeats
                gradeCount: rectangle.gradeCount
                x: 276 + (648 + 125) + (index % 8) * 40
                y: 616 - Math.floor(index / 8) * 25
                beat: 16 + (index % 8)
                row: Math.floor(index / 8)
                currentAcc: rectangle.currentAcc
                currentNoteLength: rectangle.currentNoteLength
            }
        }
        // Measure 4
        Repeater {
            model: 72
            NoteSlot {
                wrongBeats: rectangle.wrongBeats
                gradeCount: rectangle.gradeCount
                x: 276 + (972 + 200) + (index % 8) * 40
                y: 616 - Math.floor(index / 8) * 25
                beat: 24 + (index % 8)
                row: Math.floor(index / 8)
                currentAcc: rectangle.currentAcc
                currentNoteLength: rectangle.currentNoteLength
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
    //Score Text
    Text {
        x: 1650
        y: 380
        text: gradeCount > 0 ? "Score: " + currentScore + "/" + gridController.totalExpected() : ""
        font.pixelSize: 28
        color: "black"
    }
    // Bottom Buttons
    Column {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: staffLines2.bottom
        anchors.topMargin: -20
        Text {
            text: gridController.currentQuestionText
            font.pixelSize: 28
            leftPadding: 0
            color: "black"
        }
        Image {
            source: "images/gradebutton.png"
            fillMode: Image.PreserveAspectFit
            height: 40
            x: 20
            opacity: gradeArea.pressed ? 0.6 : 1.0
            MouseArea {
                id: gradeArea
                anchors.fill: parent
                onClicked: {
                    currentScore = gridController.score()
                    wrongBeats = gridController.incorrectBeats()
                    gradeCount++
                }
            }
        }
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
        Row {
            spacing: 10
            x: 35
            Image {
                source: "images/quarterbutton.png"
                fillMode: Image.PreserveAspectFit
                height: 40
                opacity: rectangle.currentNoteLength === 2 ? 0.6 : 1.0
                MouseArea {
                    anchors.fill: parent
                    onClicked: rectangle.currentNoteLength = 2
                }
            }
            Image {
                source: "images/eighthbutton.png"
                fillMode: Image.PreserveAspectFit
                height: 40
                opacity: rectangle.currentNoteLength === 1 ? 0.6 : 1.0
                MouseArea {
                    anchors.fill: parent
                    onClicked: rectangle.currentNoteLength = 1
                }
            }
        }

    }




}


