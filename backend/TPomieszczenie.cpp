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