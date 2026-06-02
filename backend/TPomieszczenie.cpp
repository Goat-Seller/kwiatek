#include "pch.h"
#include "TPomieszczenie.h"
#include <iostream>

using namespace std;

TPomieszczenie::TPomieszczenie(string nazwa, double tempPoczatkowa) { // Musi być TPomieszczenie::
    this->nazwaPomieszczenia = nazwa;
    this->temperaturaPomieszczenia = tempPoczatkowa;
}

void TPomieszczenie::dodajDoniczkePoNazwie(string nazwa) {
    TDoniczka* znaleziona = TDoniczka::znajdzDoniczke(nazwa);

    if (znaleziona != nullptr) {
        doniczki.push_back(znaleziona); // Dodajemy wskaźnik do wektora w pokoju
        cout << "Doniczka '" << nazwa << "' zostala umieszczona w: " << nazwaPomieszczenia << endl;
    }
    else {
        // Komunikat o błędzie, jeśli doniczka nie została wcześniej zadeklarowana
        cout << "Taka doniczka (" << nazwa << ") nie zostala zadeklarowana!" << endl;
    }
}

void TPomieszczenie::statusPomieszczenia() {
    cout << " POMIESZCZENIE: " << nazwaPomieszczenia << " | Temp: " << temperaturaPomieszczenia << " st. C" << endl;

    for (auto d : doniczki) {
        d->StatusDoniczkiX(temperaturaPomieszczenia);
    }
}

bool TPomieszczenie::usunDoniczkePoNazwie(std::string nazwa) {
    // Przeszukujemy wektor doniczek za pomocą iteratora
    for (auto it = doniczki.begin(); it != doniczki.end(); ++it) {
        if ((*it) != nullptr && (*it)->pobierzNazweDoniczki() == nazwa) {
            doniczki.erase(it); // Usuwamy wskaznik z listy pokoju
            return true; // Zgłaszamy sukces
        }
    }
    return false; // Nie znaleziono takiej doniczki w tym pokoju
}

void TPomieszczenie::wyswietlZawartosc() const {
    std::cout << "\n--- RAPORT POKOJU: " << nazwaPomieszczenia << " ---" << std::endl;
    std::cout << "Aktualna temperatura: " << temperaturaPomieszczenia << " st. C" << std::endl;
    std::cout << "Liczba doniczek: " << doniczki.size() << std::endl;

    if (doniczki.empty()) {
        std::cout << "  (Pokoj jest pusty)" << std::endl;
    }
    else {
        for (auto d : doniczki) {
            if (d != nullptr) {
                std::cout << "  -> " << d->pobierzNazweDoniczki();
                if (d->pobierzGatunek()) {
                    std::cout << " (Gatunek: " << d->pobierzGatunek()->pobierzNazwe() << ")";
                }
                std::cout << std::endl;
            }
        }
    }
    std::cout << "---------------------------------" << std::endl;
}