import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: "frontend"

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
            // background: "darkblue"
            // Calls the Q_INVOKABLE podlej() in the wrapper, 
            // which calls Podlewanie() in the backend.
            onClicked: DoniczkaController.podlej()
        }
    }
}