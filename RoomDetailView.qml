import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property var currentRoom: null

    signal backRequested()
    signal addPotRequested()

    // NOWOŚĆ: Wymuszenie focusu przy wejściu do pokoju, by klawiatura od razu działała
    focus: visible

    // OBSŁUGA KLAWIATURY: Powrót przez Backspace lub Escape
    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Escape || event.key === Qt.Key_Backspace) {
            root.backRequested()
            event.accepted = true // Informujemy system, że zdarzenie zostało obsłużone
        }
    }

    // Animacja symulująca upływ czasu / kalibrację
    Popup {
        id: termostatSimulationPopup
        anchors.centerIn: parent
        width: 280; height: 160
        modal: true
        closePolicy: Popup.NoAutoClose
        onOpened: simulationTimer.restart()

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 15
            BusyIndicator { Layout.alignment: Qt.AlignHCenter; running: termostatSimulationPopup.visible }
            Text { text: "Kalibracja i regulacja termostatu..."; font.bold: true; font.pointSize: 11 }
        }
    }

    Timer {
        id: simulationTimer
        interval: 1500; repeat: false
        onTriggered: {
            SystemController.uruchomTermostat(root.currentRoom.nazwa)
            termostatSimulationPopup.close()
        }
    }

    // Menu kontekstowe doniczki
    Menu {
        id: potContextMenu
        property var targetPot: null

        MenuItem {
            text: "💧 Podlej"
            onTriggered: { if (potContextMenu.targetPot) potContextMenu.targetPot.podlej() }
        }
        MenuItem {
            text: "📦 Przenieś"
            onTriggered: movePotDialog.open()
        }
        MenuItem {
            text: "🗑 Usuń"
            onTriggered: {
                if (potContextMenu.targetPot) {
                    SystemController.usunDoniczke(root.currentRoom.nazwa, potContextMenu.targetPot.nazwaDoniczki)
                }
            }
        }
    }

    // Dialog wyboru nowego pokoju 
    Dialog {
        id: movePotDialog
        anchors.centerIn: parent
        width: 360
        height: 220
        modal: true

        background: Rectangle {
            color: "#1a3a1a"
            border.color: "#3CB371"
            border.width: 2
            radius: 12
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 12
            
            // TUTAJ PRZYPINAMY KEYS (To rozwiązuje ostatni błąd z konsoli!)
            Keys.onReturnPressed: movePotDialog.accept()
            Keys.onEnterPressed: movePotDialog.accept()
            Keys.onEscapePressed: movePotDialog.reject()

            Text { 
                text: "📦 Przenieś: " + (potContextMenu.targetPot ? potContextMenu.targetPot.nazwaDoniczki : "")
                color: "white"
                font.bold: true 
                font.pointSize: 13
                Layout.bottomMargin: 5
            }
            
            ComboBox {
                id: targetRoomCombo
                Layout.fillWidth: true
                model: SystemController.pomieszczenia
                textRole: "nazwa"
                focus: true 
            }

            Item { Layout.fillHeight: true }

            // Własna, bezpieczna linia przycisków akceptacji
            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Button {
                    text: "Anuluj"
                    Layout.fillWidth: true
                    onClicked: movePotDialog.reject()
                }

                Button {
                    text: "Zapisz"
                    highlighted: true
                    Layout.fillWidth: true
                    onClicked: movePotDialog.accept()
                }
            }
        }

        onAccepted: {
            if (potContextMenu.targetPot && targetRoomCombo.currentText !== "") {
                SystemController.przeniesDoniczke(
                    potContextMenu.targetPot.nazwaDoniczki, 
                    root.currentRoom.nazwa, 
                    targetRoomCombo.currentText
                )
            }
        }
    }

    RowLayout {
        id: roomHeader
        anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right; anchors.margins: 30

        Button {
            text: "⬅ Powrót"
            onClicked: root.backRequested()
        }

        Text {
            text: root.currentRoom && root.currentRoom.temperatura !== undefined ?
                root.currentRoom.nazwa + " (" + Number(root.currentRoom.temperatura).toFixed(1) + "°C)" : ""
            color: "white"; font.pointSize: 22; font.bold: true
            Layout.fillWidth: true; horizontalAlignment: Text.AlignHCenter
        }

        Button {
            text: "🌡 Uruchom Termostat"
            highlighted: true
            onClicked: termostatSimulationPopup.open()
        }

        Button {
            text: "➕ Dodaj Doniczkę"
            highlighted: true
            onClicked: root.addPotRequested()
        }
    }

    GridLayout {
        anchors.top: roomHeader.bottom; anchors.bottom: parent.bottom
        anchors.left: parent.left; anchors.right: parent.right; anchors.margins: 40
        columns: 4; rowSpacing: 20; columnSpacing: 20

        Repeater {
            model: root.currentRoom ? root.currentRoom.doniczki : []
            DoniczkaTile {
                potName: modelData.nazwaDoniczki
                plantSpecies: modelData.nazwaGatunku
                humidity: modelData.aktualnaWilgotnosc
                stanTemperatury: modelData.stanTemperatury 
                sugerowanyPokoj: modelData.sugerowanyPokoj
                onWatered: modelData.podlej() 
                onRightClicked: {
                    potContextMenu.targetPot = modelData
                    potContextMenu.popup()
                }
            }
        }
    }
}