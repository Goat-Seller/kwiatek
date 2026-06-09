import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    signal roomSelected(var roomWrapper)
    signal addRoomRequested()
    signal manageSpeciesRequested()

    Text {
        id: headerText
        text: "Moje Pomieszczenia"
        color: "white"
        font.pointSize: 24; font.bold: true
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 30
    }

    Button {
        text: "⚙️ Opcje"
        anchors.verticalCenter: headerText.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 40
        onClicked: optionsMenu.popup()

        Menu {
            id: optionsMenu
            
            MenuItem {
                text: "🌱 Zarządzaj gatunkami"
                onTriggered: root.manageSpeciesRequested()
            }
            
            // NOWOŚĆ: Przycisk symulacji czasu schowany bezpiecznie w menu
            MenuItem {
                text: "⏳ Symuluj upływ czasu"
                onTriggered: SystemController.symulujUplywCzasu()
            }
        }
    }

    // Niewidoczne Menu kontekstowe czekające na wywołanie
    Menu {
        id: roomContextMenu
        property string targetRoom: "" // Zapamiętuje, nad którym pokojem kliknięto

        MenuItem {
            text: "🗑 Usuń pokój"
            onTriggered: {
                if (roomContextMenu.targetRoom !== "") {
                    SystemController.usunPomieszczenie(roomContextMenu.targetRoom)
                }
            }
        }
    }

    GridLayout {
        anchors.top: headerText.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 40; anchors.topMargin: 30
        columns: 3
        rowSpacing: 25; columnSpacing: 25

        Repeater {
            model: SystemController.pomieszczenia
            
            RoomTile {
                Layout.preferredWidth: 220
                Layout.preferredHeight: 150
                roomName: modelData.nazwa
                
                onClicked: root.roomSelected(modelData)
                
                // Obsługa prawego kliku dla konkretnego kafelka
                onRightClicked: {
                    roomContextMenu.targetRoom = modelData.nazwa
                    roomContextMenu.popup() // Wyświetla menu pod kursorem
                }
            }
        }

        // Kafel "Dodaj pokój"
        RoomTile {
            Layout.preferredWidth: 220
            Layout.preferredHeight: 150
            roomName: "➕ Dodaj pokój"
            onClicked: root.addRoomRequested()
        }
    }
}