import QtQuick
import GradeYourMelodyUI

Window {
    width: mainScreen.width
    height: mainScreen.height

    visible: true
    title: "GradeYourMelodyUI"

    Screen01 {
        id: mainScreen
        scale: Math.min(parent.width / Constants.width, parent.height / Constants.height)
        anchors.centerIn: parent
    }

}

