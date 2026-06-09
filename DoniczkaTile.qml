import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property string potName: ""
    property string plantSpecies: ""
    property double humidity: 0.0
    property int stanTemperatury: 0
    property string sugerowanyPokoj: ""

    signal watered()
    signal rightClicked() // <--- Nowy sygnał dla menu

    width: 180; height: 210
    radius: 12
    color: "#1a3a1a"
    border.color: root.stanTemperatury === 1 ? "#33ccff" : (root.stanTemperatury === 2 ? "#ff3300" : "#2e5c2e")
    border.width: root.stanTemperatury !== 0 ? 3 : 2

    // Łapacz prawego kliknięcia (leży pod przyciskiem, więc nie psuje jego działania)
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: (mouse) => {
            if (mouse.button === Qt.RightButton) {
                root.rightClicked()
            }
        }
    }

    // IKONA w prawym górnym rogu
    Text {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        text: root.stanTemperatury === 1 ? "❄️" : (root.stanTemperatury === 2 ? "🔥" : "")
        font.pointSize: 18
        visible: root.stanTemperatury !== 0
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 6

        Text { 
            text: root.potName
            color: "white"
            font.bold: true; font.pointSize: 14
            Layout.alignment: Qt.AlignHCenter 
        }
        Text { 
            text: "Gatunek: " + root.plantSpecies; 
            color: "#a0c0a0"; font.pointSize: 10 
            Layout.alignment: Qt.AlignHCenter 
        }
        Text { 
            text: "Wilgotność: " + root.humidity.toFixed(1) + "%"
            color: root.humidity < 30 ? "#ff6666" : "#66ff66"
            font.bold: true 
            Layout.alignment: Qt.AlignHCenter 
        }
        
        // TEKST SUGESTII (pojawia się tylko po teście termostatu)
        Text {
            text: root.sugerowanyPokoj !== "" ? "Przenieś do:\n" + root.sugerowanyPokoj : (root.stanTemperatury !== 0 ? "Brak odpowiedniego\npokoju!" : "")
            color: root.stanTemperatury === 1 ? "#88ddff" : "#ffaa00"
            font.pointSize: 9
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            visible: root.stanTemperatury !== 0
            Layout.alignment: Qt.AlignHCenter
        }

        Item { Layout.fillHeight: true } // Pusty element dociskający przycisk do dołu

        Button {
            text: "Podlej"
            Layout.fillWidth: true
            onClicked: root.watered()
        }
    }
}