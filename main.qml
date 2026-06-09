import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true; width: 900; height: 700
    title: qsTr("System Zarządzania Roślinami")
    color: "#001a00"

    property int currentView: 0
    property var activeRoomWrapper: null // Przechowuje wskaźnik na wybrany pokój

   // WIDOK 0: Panel główny (Siatka pokoi)
   MainHubView {
        anchors.fill: parent
        visible: window.currentView === 0
        onRoomSelected: function(roomWrapper) {
            window.activeRoomWrapper = roomWrapper
            window.currentView = 1 
        }
        onAddRoomRequested: addRoomDialog.open()
        
        // Przełączenie na widok zarządzania gatunkami
        onManageSpeciesRequested: window.currentView = 2 
    }

    // WIDOK 1: Szczegóły pomieszczenia
    RoomDetailView {
        anchors.fill: parent
        visible: window.currentView === 1
        currentRoom: window.activeRoomWrapper
        
        onBackRequested: window.currentView = 0
        onAddPotRequested: addPotDialog.open()
    }
    // WIDOK 2: Panel zarządzania gatunkami roślin
    SpeciesManagementView {
        anchors.fill: parent
        visible: window.currentView === 2
        onBackRequested: window.currentView = 0
    }


    // Dialog: Dodawanie doniczki z listą gatunków z bazy
    Dialog {
        id: addPotDialog
        anchors.centerIn: parent
        width: 380
        height: 340 // Dostosowana wysokość do własnego ukłądu przycisków
        modal: true

        background: Rectangle {
            color: "#1a3a1a"
            border.color: "#3CB371"
            border.width: 2
            radius: 12
            // Brak wstrzykniętych paneli sprawia, że ten prostokąt idealnie ogranicza całe okno
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 12

            // Bezpieczne przypięcie klawiszy do Itemu (likwiduje błędy w konsoli)
            Keys.onReturnPressed: addPotDialog.accept()
            Keys.onEnterPressed: addPotDialog.accept()
            Keys.onEscapePressed: addPotDialog.reject()

            // Nasz własny, elegancki nagłówek
            Text {
                text: "✨ Dodaj roślinę do pokoju"
                color: "white"
                font.bold: true
                font.pointSize: 14
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 5
            }

            Text { 
                text: "Nazwa własna doniczki:"
                color: "#a0c0a0"
                font.bold: true 
            }
            TextField { 
                id: potNameInput
                placeholderText: "np. Monitorowany Fikus"
                Layout.fillWidth: true
                color: "white"
                focus: true
                
                background: Rectangle {
                    color: "#001a00"
                    border.color: parent.activeFocus ? "#3CB371" : "#2e5c2e"
                    radius: 5
                }
                placeholderTextColor: "#557755"
            }

            Text { 
                text: "Wybierz gatunek z bazy:"
                color: "#a0c0a0"
                font.bold: true 
            }
            
            ComboBox {
                id: speciesComboBox
                Layout.fillWidth: true
                model: SystemController.gatunki
                
                contentItem: Text {
                    text: speciesComboBox.currentText
                    color: "white"
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                }

                background: Rectangle {
                    color: "#001a00"
                    border.color: "#2e5c2e"
                    radius: 5
                }
                
                popup: Popup {
                    y: speciesComboBox.height + 2
                    width: speciesComboBox.width
                    implicitHeight: contentItem.implicitHeight > 110 ? 110 : contentItem.implicitHeight
                    padding: 1

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight
                        model: speciesComboBox.popup.visible ? speciesComboBox.delegateModel : null
                        ScrollIndicator.vertical: ScrollIndicator { }
                    }

                    background: Rectangle {
                        color: "#1a3a1a"
                        border.color: "#3CB371"
                        radius: 5
                    }
                }

                delegate: ItemDelegate {
                    width: speciesComboBox.width
                    contentItem: Text {
                        text: modelData
                        color: "white"
                        font.bold: highlighted
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: highlighted ? "#2E8B57" : "transparent"
                    }
                }
            }

            Item { Layout.fillHeight: true }

            // NASZE WŁASNE PRZYCISKI - pasujące do stylu i niezasłaniające widoku
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                Layout.topMargin: 5

                Button {
                    text: "Anuluj"
                    Layout.fillWidth: true
                    onClicked: addPotDialog.reject()
                }

                Button {
                    text: "Zapisz"
                    highlighted: true
                    Layout.fillWidth: true
                    onClicked: addPotDialog.accept()
                }
            }
        }

        onAccepted: {
            if (window.activeRoomWrapper && potNameInput.text !== "") {
                SystemController.dodajDoniczkeDoPokoju(
                    window.activeRoomWrapper.nazwa, 
                    potNameInput.text, 
                    speciesComboBox.currentText
                )
            }
            potNameInput.text = ""
        }
        
        onRejected: {
            potNameInput.text = ""
        }
    }

    // Dialog: Tworzenie nowego pomieszczenia
   Dialog {
        id: addRoomDialog
        anchors.centerIn: parent
        width: 380
        height: 320 // Sztywna wysokość zapobiegająca przesuwaniu przycisków
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

            // Bezpieczne i bezbłędne przypięcie obsługi klawiatury do Itemu
            Keys.onReturnPressed: addRoomDialog.accept()
            Keys.onEnterPressed: addRoomDialog.accept()
            Keys.onEscapePressed: addRoomDialog.reject()

            // Nasz spersonalizowany nagłówek
            Text {
                text: "✨ Utwórz nowe pomieszczenie"
                color: "white"
                font.bold: true
                font.pointSize: 14
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 5
            }

            Text { 
                text: "Nazwa pokoju:"
                color: "#a0c0a0"
                font.bold: true 
            }
            TextField { 
                id: newRoomNameInput
                placeholderText: "np. Sypialnia, Biuro..."
                Layout.fillWidth: true
                color: "white"
                focus: true
                KeyNavigation.tab: newRoomTempInput
                
                background: Rectangle {
                    color: "#001a00"
                    border.color: parent.activeFocus ? "#3CB371" : "#2e5c2e"
                    radius: 5
                }
                placeholderTextColor: "#557755"
            }

            Text { 
                text: "Aktualna temperatura (°C):"
                color: "#a0c0a0"
                font.bold: true 
            }
            SpinBox { 
                id: newRoomTempInput
                from: 10; to: 40
                value: 22
                editable: true

                KeyNavigation.tab: newRoomNameInput
                
                // Stylizacja tekstu wewnątrz SpinBoxa, aby był widoczny w ciemnym motywie
                contentItem: TextInput {
                    text: newRoomTempInput.textFromValue(newRoomTempInput.value, newRoomTempInput.locale)
                    font: newRoomTempInput.font
                    color: "white"
                    selectionColor: "#2E8B57"
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    readOnly: !newRoomTempInput.editable
                    validator: newRoomTempInput.validator
                    inputMethodHints: Qt.ImhFormattedNumbersWidth
                }

                background: Rectangle {
                    color: "#001a00"
                    border.color: "#2e5c2e"
                    radius: 5
                }
            }

            Item { Layout.fillHeight: true }

            // Własne zaokrąglone przyciski akcji na dole okna
            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Button {
                    text: "Anuluj"
                    Layout.fillWidth: true
                    onClicked: addRoomDialog.reject()
                }

                Button {
                    text: "Zapisz"
                    highlighted: true
                    Layout.fillWidth: true
                    onClicked: addRoomDialog.accept()
                }
            }
        }

        onAccepted: {
            if (newRoomNameInput.text !== "") {
                var newRoom = SystemController.dodajPomieszczenie(newRoomNameInput.text, newRoomTempInput.value)
                if (newRoom) {
                    window.activeRoomWrapper = newRoom 
                    window.currentView = 1 
                }
            }
            newRoomNameInput.text = "" 
            newRoomTempInput.value = 22 
        }
        
        onRejected: {
            newRoomNameInput.text = "" 
            newRoomTempInput.value = 22
        }
    }
}