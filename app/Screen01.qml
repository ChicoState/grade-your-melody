


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
    property int currentNoteLength: 1 // 1 = eighth note, 2 = quarter note, 3 = half note, 4 = whole note
    property int currentScore: 0
    property var wrongBeats: []
    property int gradeCount: 0 // increments each time grade is clicked
    property bool showAnswer: false
    // Three top-level modes: Quiz (default, both flags false), Ear Training, Free Staff.
    // Mutual exclusion is enforced inside the onXxxChanged handlers below.
    property bool earTraining: false
    property bool earAnswerSelected: false
    property bool earAnswerCorrect: false
property bool freeStaff: false

readonly property color accentBlue: rectangle.accentBlue
readonly property color mutedGray: rectangle.mutedGray
readonly property color errorRed: rectangle.errorRed
readonly property color successGreen: rectangle.successGreen
readonly property color cardBg: "#F7F7F7"
readonly property color cardBorder: "#D0D0D0"

onCurrentAccChanged: console.log("currentAcc now", currentAcc)

    // Reset Ear Training and answer state when toggling the mode
    onEarTrainingChanged: {
        showAnswer        = false
        earAnswerSelected = false
        earAnswerCorrect  = false
        gridController.stopPlayback()
        if (earTraining) freeStaff = false   // mutual exclusion
    }

    // Toggle Free Staff mode in the controller and reset overlapping state
    onFreeStaffChanged: {
        showAnswer        = false
        earAnswerSelected = false
        earAnswerCorrect  = false
        gridController.stopPlayback()
        gridController.setFreeStaffMode(freeStaff)
        if (freeStaff) earTraining = false   // mutual exclusion
    }

    // Reset local score state whenever the controller loads a new question
    Connections {
        target: gridController
        function onQuestionChanged() {
            currentScore      = 0
            wrongBeats        = []
            gradeCount        = 0
            showAnswer        = false
            earAnswerSelected = false
            earAnswerCorrect  = false
            gridController.stopPlayback()
        }
    }

    // Top-level mode toggle
    Row {
        id: modeToggle
        x: 30
        y: 20
        spacing: 30

        Text {
            text: "Quiz Mode"
            font.pixelSize: 22
            font.bold: !rectangle.earTraining && !rectangle.freeStaff
            color: (!rectangle.earTraining && !rectangle.freeStaff) ? "#1565C0" : "#888888"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectangle.earTraining = false
                    rectangle.freeStaff   = false
                }
            }
        }
        Text {
            text: "Ear Training"
            font.pixelSize: 22
            font.bold: rectangle.earTraining
            color: rectangle.earTraining ? "#1565C0" : "#888888"
            MouseArea {
                anchors.fill: parent
                onClicked: rectangle.earTraining = true
            }
        }
        Text {
            text: "Free Staff"
            font.pixelSize: 22
            font.bold: rectangle.freeStaff
            color: rectangle.freeStaff ? "#1565C0" : "#888888"
            MouseArea {
                anchors.fill: parent
                onClicked: rectangle.freeStaff = true
            }
        }
    }

    Image {
        id: staffLines2
        x: 53
        y: 403
        width: 1814
        height: 275
        source: "images/stafflines_two_measures.png"
        fillMode: Image.PreserveAspectFit
    }
    Item {
        x: 0
        y: 0
        width: 1920
        height: 1080

        // Measure 1 (8 slots × 14 rows = 112)
        Repeater {
            model: 184
            NoteSlot {
                wrongBeats: rectangle.wrongBeats
                gradeCount: rectangle.gradeCount
                showAnswer: rectangle.showAnswer
                earTraining: rectangle.earTraining
                x: 276 + (index % 8) * 90
                y: 666 - Math.floor(index / 8) * 25
                beat: index % 8
                row: Math.floor(index / 8)
                currentAcc: rectangle.currentAcc
                currentNoteLength: rectangle.currentNoteLength
            }
        }
        // Measure 2
        Repeater {
            model: 112
            NoteSlot {
                wrongBeats: rectangle.wrongBeats
                gradeCount: rectangle.gradeCount
                showAnswer: rectangle.showAnswer
                earTraining: rectangle.earTraining
                x: 276 + (648 + 125) + (index % 8) * 90
                y: 666 - Math.floor(index / 8) * 25
                beat: 8 + (index % 8)
                row: Math.floor(index / 8)
                currentAcc: rectangle.currentAcc
                currentNoteLength: rectangle.currentNoteLength
            }
        }
    }

    Image {
        id: title
        x: 447
        y: 240
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
            // In Ear Training the real question text would reveal the answer ("Write a F major scale"),
            // so we hide it behind a neutral label. Free Staff has no question, so we show a mode label too.
            text: rectangle.freeStaff   ? "Free Staff"
                : rectangle.earTraining ? "Ear Training: Identify the Sound"
                : gridController.currentQuestionText
            font.pixelSize: 28
            color: "black"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // ── Free Staff: Clear Staff button (visible only in Free Staff mode) ─
        Text {
            text: "Clear Staff"
            font.pixelSize: 22
            color: clearStaffArea.containsPress ? "#888888" : "#C62828"
            anchors.horizontalCenter: parent.horizontalCenter
            visible: rectangle.freeStaff
            MouseArea {
                id: clearStaffArea
                anchors.fill: parent
                onClicked: gridController.clearStaff()
            }
        }

        // ── Ear Training controls (visible only in Ear Training mode) ────────
        Column {
            spacing: 16
            anchors.horizontalCenter: parent.horizontalCenter
            visible: rectangle.earTraining

            Text {
                text: "🔊 Hear Question"
                font.pixelSize: 24
                color: hearArea.containsPress ? "#888888" : "#1565C0"
                anchors.horizontalCenter: parent.horizontalCenter
                MouseArea {
                    id: hearArea
                    anchors.fill: parent
                    onClicked: gridController.playExpectedAnswer()
                }
            }

            Row {
                spacing: 40
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    text: gridController.currentChoiceA
                    font.pixelSize: 24
                    color: choiceAArea.containsPress ? "#888888" : "black"
                    opacity: rectangle.earAnswerSelected ? 0.5 : 1.0
                    MouseArea {
                        id: choiceAArea
                        anchors.fill: parent
                        enabled: !rectangle.earAnswerSelected
                        onClicked: {
                            rectangle.earAnswerSelected = true
                            rectangle.earAnswerCorrect  = (gridController.currentCorrectChoice === "A")
                            rectangle.showAnswer        = true
                        }
                    }
                }
                Text {
                    text: gridController.currentChoiceB
                    font.pixelSize: 24
                    color: choiceBArea.containsPress ? "#888888" : "black"
                    opacity: rectangle.earAnswerSelected ? 0.5 : 1.0
                    MouseArea {
                        id: choiceBArea
                        anchors.fill: parent
                        enabled: !rectangle.earAnswerSelected
                        onClicked: {
                            rectangle.earAnswerSelected = true
                            rectangle.earAnswerCorrect  = (gridController.currentCorrectChoice === "B")
                            rectangle.showAnswer        = true
                        }
                    }
                }
            }

            Text {
                visible: rectangle.earAnswerSelected
                text: rectangle.earAnswerCorrect ? "Correct!" : "Incorrect"
                font.pixelSize: 28
                color: rectangle.earAnswerCorrect ? "#2E7D32" : "#C62828"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Image {
            source: "images/gradebutton.png"
            fillMode: Image.PreserveAspectFit
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !rectangle.earTraining && !rectangle.freeStaff
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
        Text {
            text: rectangle.showAnswer ? "◀ Hide Answer" : "Show Answer ▶"
            font.pixelSize: 20
            color: showAnswerArea.containsPress ? "#888888" : "#1565C0"
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !rectangle.earTraining && !rectangle.freeStaff
            MouseArea {
                id: showAnswerArea
                anchors.fill: parent
                onClicked: rectangle.showAnswer = !rectangle.showAnswer
            }
        }
        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !rectangle.earTraining

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
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !rectangle.earTraining
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
                source: "images/halfbutton.png"
                fillMode: Image.PreserveAspectFit
                height: 40
                opacity: rectangle.currentNoteLength === 3 ? 0.6 : 1.0
                MouseArea {
                    anchors.fill: parent
                    onClicked: rectangle.currentNoteLength = 3
                }
            }
            Image {
                source: "images/wholebutton.png"
                fillMode: Image.PreserveAspectFit
                height: 40
                opacity: rectangle.currentNoteLength === 4 ? 0.6 : 1.0
                MouseArea {
                    anchors.fill: parent
                    onClicked: rectangle.currentNoteLength = 4
                }
            }

        }

        // Audio playback
        Row {
            spacing: 30
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !rectangle.earTraining

            Text {
                text: "▶ Play"
                font.pixelSize: 24
                color: playArea.containsPress ? "#888888" : "black"
                MouseArea {
                    id: playArea
                    anchors.fill: parent
                    onClicked: gridController.playCurrentNotes()
                }
            }

            Text {
                text: "■ Stop"
                font.pixelSize: 24
                color: stopArea.containsPress ? "#888888" : "black"
                MouseArea {
                    id: stopArea
                    anchors.fill: parent
                    onClicked: gridController.stopPlayback()
                }
            }
        }

        // Tempo control
        Row {
            spacing: 16
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "−"
                font.pixelSize: 28
                color: tempoMinusArea.containsPress ? "#888888" : "black"
                MouseArea {
                    id: tempoMinusArea
                    anchors.fill: parent
                    onClicked: gridController.decreaseTempo()
                }
            }

            Text {
                text: "Tempo: " + gridController.tempoBpm + " BPM"
                font.pixelSize: 24
                color: "black"
            }

            Text {
                text: "+"
                font.pixelSize: 28
                color: tempoPlusArea.containsPress ? "#888888" : "black"
                MouseArea {
                    id: tempoPlusArea
                    anchors.fill: parent
                    onClicked: gridController.increaseTempo()
                }
            }
        }

        // Question navigation
        Row {
            spacing: 30
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !rectangle.freeStaff

            Text {
                text: "◀ Back"
                font.pixelSize: 24
                color: backArea.containsPress ? "#888888" : "black"
                MouseArea {
                    id: backArea
                    anchors.fill: parent
                    onClicked: gridController.previousQuestion()
                }
            }

            Text {
                text: "Question " + gridController.currentQuestionNum + " / " + gridController.totalQuestionsAvailable()
                font.pixelSize: 24
                color: "black"
            }

            Text {
                text: "Next ▶"
                font.pixelSize: 24
                color: nextArea.containsPress ? "#888888" : "black"
                MouseArea {
                    id: nextArea
                    anchors.fill: parent
                    onClicked: gridController.nextQuestion()
                }
            }
        }
    }

    // ── Playback beat cursor ─────────────────────────────────────────────────
    // Deliberately extreme size/color/z to confirm the signal chain is working.
    // Dial back once confirmed visible.
    Rectangle {
        id: playbackCursor
        visible: gridController.currentPlaybackBeat >= 0
        z: 9999
        opacity: 0.75

        // Column x: Measure 1 beats 0–7, Measure 2 beats 8–15
        x: {
            const b = gridController.currentPlaybackBeat
            if (b < 0) return 0
            return b < 8 ? 276 + b * 90
                         : 1049 + (b - 8) * 90
        }
        y: 0
        width: 120
        height: 900

        color: "red"
        radius: 0
    }

    // ── Beat cursor diagnostic ───────────────────────────────────────────────
    // Logs currentPlaybackBeat to the console so you can confirm it's updating.
    // Remove once verified working.
    Connections {
        target: gridController
        function onPlaybackBeatChanged() {
            console.log("[PlaybackCursor] beat →", gridController.currentPlaybackBeat)
        }
    }
}



