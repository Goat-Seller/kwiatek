import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    visible: true
    width: 640
    height: 480
    title: "Projekt - kwiatek"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: "Nazwa: " + DoniczkaController.nazwaDoniczki
            font.pointSize: 20
        }

        Text {
            text: "Wilgotność: " + DoniczkaController.aktualnaWilgotnosc + "%"
            font.pointSize: 16
        }

        Button {
            text: "Podlej"
            palette.button: "darkgreen"
            palette.buttonText: "white"

            onClicked: DoniczkaController.podlej()
        }
    }
}