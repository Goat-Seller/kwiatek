#include "pch.h"
#include "BazaRoslin.h"
#include <iostream>

int main() {
    std::cout << "--- TEST ODCZYTU Z PLIKU JSON ---" << std::endl;

    // Podpinamy się pod plik wygenerowany w poprzednim teście
    BazaRoslin baza("baza_relacyjna.json");

    if (baza.wczytajZPliku()) {
        std::cout << "[SUKCES] Plik wczytany poprawnie!" << std::endl;

        // Sprawdzamy, czy wskaźniki poprawnie połączyły doniczkę z gatunkiem
        const TGatunek* testGatunku = baza.pobierzGatunek("Kaktus");
        if (testGatunku != nullptr) {
            std::cout << "Znalazlem w pamieci gatunek Kaktus. Wymaga on: "
                << testGatunku->pobierzMinWilgotnosc() << "% wilgotnosci." << std::endl;
        }
        else {
            std::cout << "[BLAD] Nie udalo sie odtworzyc gatunku!" << std::endl;
        }
    }
    else {
        std::cout << "[BLAD] Nie moge otworzyc pliku baza_relacyjna.json!" << std::endl;
    }

    return 0;
}