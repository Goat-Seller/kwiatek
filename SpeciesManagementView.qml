import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    signal backRequested()

    property string selectedSpecies: ""

    // Funkcja czyszcząca formularz
    function resetForm() {
        selectedSpecies = ""
        speciesNameInput.text = ""
        minHumInput.value = 30
        targetTempInput.value = 22
        speciesNameInput.readOnly = false
    }

    // Nagłówek
    RowLayout {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 30

        Button {
            text: "⬅ Powrót"
            onClicked: {
                root.resetForm()
                root.backRequested()
            }
        }

        Text {
            text: "Zarządzanie Słownikiem Gatunków"
            color: "white"; font.pointSize: 22; font.bold: true
            Layout.fillWidth: true; horizontalAlignment: Text.AlignHCenter
        }
        
        // Pusty element dla wyśrodkowania tytułu
        Item { Layout.preferredWidth: 80 } 
    }

    RowLayout {
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 40; anchors.topMargin: 20
        spacing: 30

        // LEWA STRONA: Lista gatunków
        Rectangle {
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            color: "#142d14"
            radius: 10
            border.color: "#2e5c2e"

            ScrollView {
                anchors.fill: parent; anchors.margins: 10
                clip: true

                ListView {
                    id: speciesListView
                    model: SystemController.gatunki // Zwraca QStringList z C++
                    spacing: 5

                    delegate: Rectangle {
                        width: speciesListView.width
                        height: 45
                        radius: 6
                        color: root.selectedSpecies === modelData ? "#2E8B57" : (mouseArea.containsMouse ? "#1e3e1e" : "transparent")
                        
                        Text {
                            anchors.left: parent.left; anchors.leftMargin: 15
                            anchors.verticalCenter: parent.verticalCenter
                            text: modelData
                            color: "white"
                            font.bold: true; font.pixelSize: 15
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                root.selectedSpecies = modelData
                                speciesNameInput.text = modelData
                                speciesNameInput.readOnly = true // Blokada edycji klucza głównego
                                minHumInput.value = SystemController.getGatunekMinHumidity(modelData)
                                targetTempInput.value = SystemController.getGatunekTemperature(modelData)
                            }
                        }
                    }
                }
            }
        }

        // PRAWA STRONA: Formularz edycji / tworzenia
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1a3a1a"
            radius: 10
            border.color: "#3CB371"
            border.width: 1

            // Skróty globalne dla całego formularza
            Keys.onReturnPressed: saveButton.clicked()
            Keys.onEnterPressed: saveButton.clicked()
            Keys.onEscapePressed: root.backRequested()

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 30
                spacing: 20

                Text {
                    text: root.selectedSpecies !== "" ? "⚙️ Tryb Edycji Gatunku" : "✨ Dodaj Nowy Gatunek"
                    color: "white"; font.pointSize: 16; font.bold: true
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: "#2e5c2e" }

                // Pola formularza z przypisaną nawigacją klawisza TAB
                ColumnLayout {
                    Layout.fillWidth: true; spacing: 8
                    Text { text: "Nazwa Gatunku:"; color: "white"; font.bold: true }
                    TextField {
                        id: speciesNameInput
                        placeholderText: "np. Orchidea, Monstera..."
                        Layout.fillWidth: true
                        focus: true // Domyślny start focusu
                        KeyNavigation.tab: minHumInput // Tab kieruje do pola niżej
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true; spacing: 8
                    Text { text: "Minimalna wymagana wilgotność (%):"; color: "white"; font.bold: true }
                    SpinBox {
                        id: minHumInput
                        from: 0; to: 100
                        editable: true
                        KeyNavigation.tab: targetTempInput // Tab kieruje do pola niżej
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true; spacing: 8
                    Text { text: "Optymalna temperatura (°C):"; color: "white"; font.bold: true }
                    SpinBox {
                        id: targetTempInput
                        from: 5; to: 50
                        editable: true
                        KeyNavigation.tab: speciesNameInput // Tab wraca na samą górę (pętla)
                    }
                }

                Item { Layout.fillHeight: true }

                // Akcje formularza
                RowLayout {
                    Layout.fillWidth: true; spacing: 15

                    Button {
                        text: "Wyczyść / Nowy"
                        Layout.fillWidth: true
                        onClicked: root.resetForm()
                    }

                    Button {
                        id: saveButton // Dodane ID do wywołania przez skrót klawiszowy
                        text: "Zapisz"
                        highlighted: true
                        Layout.fillWidth: true
                        onClicked: {
                            if (speciesNameInput.text.trim() === "") return;
                            if (root.selectedSpecies !== "") {
                                SystemController.aktualizujGatunek(speciesNameInput.text, minHumInput.value, targetTempInput.value)
                            } else {
                                SystemController.dodajGatunek(speciesNameInput.text, minHumInput.value, targetTempInput.value)
                            }
                            root.resetForm()
                        }
                    }

                    Button {
                        text: "Usuń Gatunek"
                        enabled: root.selectedSpecies !== ""
                        Layout.fillWidth: true
                        onClicked: {
                            if (root.selectedSpecies !== "") {
                                SystemController.usunGatunek(root.selectedSpecies)
                                root.resetForm()
                            }
                        }
                    }
                }
            }
        }
    }
}