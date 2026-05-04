import QtQuick
import QtQuick.Controls
import QtMultimedia
import GradeYourMelodyUI

Rectangle {
    id: rectangle
    width: 1920
    height: 1100
    color: "#EAEAEA"
    clip: true

    property var occupiedBeats: [false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false]
    property int currentAcc: 0
    property int currentNoteLength: 2
    property int currentScore: 0
    property var wrongBeats: []
    property int gradeCount: 0
    property string questionText: ""

    SoundEffect {
        id: noteC4
        source: "qrc:/qt/qml/GradeYourMelodyUI/sounds/Piano_C4.wav"
        volume: 0.8
    }

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

    Text {
        x: 1650
        y: 380
        text: gradeCount > 0 ? "Score: " + currentScore + "/" + gridController.totalExpected() : ""
        font.pixelSize: 28
        color: "black"
    }

    Column {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: staffLines2.bottom
        anchors.topMargin: -20

        Text {
            text: questionText
            font.pixelSize: 28
            leftPadding: 0
            color: "black"
        }

        Button {
            text: "Grade"
            onClicked: {
                currentScore = gridController.score()
                wrongBeats = gridController.incorrectBeats()
                gradeCount++
            }
        }

        Row {
            spacing: 10

            Button {
                text: "Flat"
                onClicked: rectangle.currentAcc = -1
            }

            Button {
                text: "Natural"
                onClicked: rectangle.currentAcc = 0
            }

            Button {
                text: "Sharp"
                onClicked: rectangle.currentAcc = 1
            }
        }

        Row {
            spacing: 10
            x: 35

            Button {
                text: "Quarter"
                onClicked: {
                    rectangle.currentNoteLength = 2
                    noteC4.play()
                }
            }

            Button {
                text: "Eighth"
                onClicked: rectangle.currentNoteLength = 1
            }
        }
    }
}