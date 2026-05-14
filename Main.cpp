#include "pch.h" // Pamiętaj o tym, jeśli dalej macie prekompilowane nagłówki!
#include <iostream>
#include <string>
#include "TDoniczka.h"
#include "TPomieszczenie.h"
#include "BazaRoslin.h"

using namespace std;

int main() {
    TDoniczka d1("Kaktus Marka", "Kaktus");
    TDoniczka d2("Paproc Ani", "Paproc");

    TPomieszczenie salon("Salon", 22.0);

    salon.dodajDoniczkePoNazwie("Kaktus Marka");
    salon.dodajDoniczkePoNazwie("Storczyk");
    salon.statusPomieszczenia();

    // 2. TWOJA CZĘŚĆ: Ładowanie bazy danych
    BazaRoslin baza("baza.json");
    if (!baza.wczytajZPliku()) {
        cout << "Blad wczytywania bazy. Upewnij sie, ze plik baza.json lezy w folderze z projektem!" << endl;
    }
    else {
        cout << "Wczytano baze roslin (JSON) pomyslnie!" << endl;
    }


    // 3. TWOJA CZĘŚĆ: Pętla komend wpisywanych przez użytkownika
    cout << "\n--- PANEL STEROWANIA ---" << endl;
    cout << "Dostepne komendy: lista, info <id>, podlej <id>, wyjscie\n";

    string komenda;
    string argument;

    while (true) {
        cout << "\nWpisz komende: ";
        cin >> komenda;

        if (komenda == "lista") {
            baza.wyswietlWszystkie();
        }
        else if (komenda == "info") {
            cin >> argument;
            baza.sprawdzRosline(argument);
        }
        else if (komenda == "podlej") {
            cin >> argument;
            baza.podlej(argument);
        }
        else if (komenda == "wyjscie") {
            baza.zapiszDoPliku();
            cout << "Zamykanie programu..." << endl;
            break;
        }
        else {
            cout << "Nieznana komenda. Dostepne to: lista, info, podlej, wyjscie." << endl;
            cin.clear();
            cin.ignore(10000, '\n'); // Czyszczenie śmieci z bufora
        }
    }

    return 0;
}