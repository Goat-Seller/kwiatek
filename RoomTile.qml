import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    
    property string roomName: "Dodaj pokój"
    property bool isOccupied: false
    
    signal clicked()
    signal rightClicked() // <--- Nowy sygnał dla prawego przycisku

    color: mouseArea.containsMouse ? "#2E8B57" : "#004d00" 
    radius: 10
    border.color: "#3CB371"
    border.width: 2

    Behavior on color { ColorAnimation { duration: 150 } }

    Text {
        anchors.centerIn: parent
        text: root.roomName
        color: "white"
        font.pixelSize: 18
        font.bold: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true 
        cursorShape: Qt.PointingHandCursor
        
        // Deklarujemy, że przyjmujemy kliknięcia z obu przycisków
        acceptedButtons: Qt.LeftButton | Qt.RightButton 
        
        onClicked: (mouse) => {
            if (mouse.button === Qt.RightButton) {
                root.rightClicked() // Prawy przycisk -> sygnał kontekstowy
            } else {
                root.clicked()      // Lewy przycisk -> standardowe wejście
            }
        }
    }
}