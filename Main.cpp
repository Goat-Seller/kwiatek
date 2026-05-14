#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "TDoniczka.h"
#include "BazaRoslin.h"

using namespace std;

int main() {
    // 1. LADOWANIE ENCYKLOPEDII (baza.json)
    BazaRoslin baza("baza.json");
    if (!baza.wczytajZPliku()) {
        cout << "[BLAD] Nie mozna wczytac baza.json!" << endl;
    }
    else {
        cout << "Wczytano baze gatunkow pomyslnie." << endl;
    }

    // 2. LADOWANIE FIZYCZNYCH DONICZEK (doniczki.json)
    vector<TDoniczka> mojeDoniczki;
    std::ifstream plik_doniczki("doniczki.json");

    if (plik_doniczki.is_open()) {
        nlohmann::json j_doniczki;
        plik_doniczki >> j_doniczki;
        plik_doniczki.close();

        for (const auto& d : j_doniczki) {
            string nazwaGatunku = d["gatunek"];
            Roslina odtworzonaZJsona;

            // Jesli gatunek istnieje w JSON, tworzymy doniczke z 4 ARGUMENTAMI
            if (baza.pobierzDaneRosliny(nazwaGatunku, odtworzonaZJsona)) {
                TDoniczka wczytana(d["nazwa"], odtworzonaZJsona.nazwa, odtworzonaZJsona.wilgotnosc_min, 22.0);
                mojeDoniczki.push_back(wczytana);
            }
        }
        cout << "Wczytano " << mojeDoniczki.size() << " doniczek z pliku doniczki.json." << endl;
    }
    else {
        cout << "Brak pliku doniczki.json. Zaczynamy z pustym domem." << endl;
    }

    // --- PANEL STEROWANIA ---
    cout << "\n=== SYSTEM SMART-HOME GOTOWY ===" << endl;
    cout << "Dostepne komendy:" << endl;
    cout << "  lista                     - spis gatunkow w encyklopedii" << endl;
    cout << "  wymagania <id>            - szczegoly gatunku z bazy" << endl;
    cout << "  dodaj <nazwa> <id>        - stworz nowa doniczke" << endl;
    cout << "  status                    - pokaz stan wszystkich Twoich doniczek" << endl;
    cout << "  info <nazwa_doniczki>     - szczegoly konkretnej doniczki" << endl;
    cout << "  podlej <nazwa_doniczki>   - podlej fizyczna doniczke" << endl;
    cout << "  wyjscie                   - zapisz wszystko i zamknij" << endl;

    string komenda, arg1, arg2;

    while (true) {
        cout << "\nWpisz komende: ";
        cin >> komenda;

        if (komenda == "lista") {
            baza.wyswietlWszystkie();
        }
        else if (komenda == "wymagania") {
            cin >> arg1;
            baza.sprawdzRosline(arg1);
        }
        else if (komenda == "dodaj") {
            cin >> arg1 >> arg2;

            Roslina daneZJsona;

            // Weryfikacja z JSON + pobranie wszystkich danych (wilgotnosc itp.)
            if (baza.pobierzDaneRosliny(arg2, daneZJsona)) {
                // Tworzenie doniczki z 4 ARGUMENTAMI (przekazujemy dane z JSON-a do klasy kolegi)
                TDoniczka nowa(arg1, daneZJsona.nazwa, daneZJsona.wilgotnosc_min, 22.0);
                mojeDoniczki.push_back(nowa);
                cout << "-> Sukces: Dodano doniczke '" << arg1 << "' (Gatunek: " << daneZJsona.nazwa << ")" << endl;
            }
            else {
                cout << "[BLAD] Nie dodano! '" << arg2 << "' nie figuruje w bazie gatunkow." << endl;
            }
        }
        else if (komenda == "status") {
            if (mojeDoniczki.empty()) {
                cout << "Nie masz jeszcze zadnych doniczek." << endl;
            }
            else {
                for (const auto& d : mojeDoniczki) {
                    d.StatusDoniczkiX();
                }
            }
        }
        else if (komenda == "info") {
            cin >> arg1;
            bool znaleziono = false;
            for (const auto& d : mojeDoniczki) {
                if (d.pobierzNazwe() == arg1) {
                    cout << "\n>>> SZCZEGOLY DONICZKI: " << arg1 << " <<<" << endl;
                    d.StatusDoniczkiX();
                    znaleziono = true;
                    break;
                }
            }
            if (!znaleziono) cout << "[BLAD] Nie znaleziono doniczki o nazwie: " << arg1 << endl;
        }
        else if (komenda == "podlej") {
            cin >> arg1;
            bool znaleziono = false;
            for (auto& d : mojeDoniczki) {
                if (d.pobierzNazwe() == arg1) {
                    cout << ">>> PODLEWANIE DONICZKI: " << arg1 << " <<<" << endl;
                    d.Podlewanie();
                    baza.podlej(d.pobierzGatunek());
                    znaleziono = true;
                    break;
                }
            }
            if (!znaleziono) cout << "[BLAD] Brak doniczki: " << arg1 << endl;
        }
        else if (komenda == "wyjscie") {
            baza.zapiszDoPliku();

            nlohmann::json j_zapis_doniczek = nlohmann::json::array();
            for (const auto& d : mojeDoniczki) {
                nlohmann::json temp;
                temp["nazwa"] = d.pobierzNazwe();
                temp["gatunek"] = d.pobierzGatunek();
                j_zapis_doniczek.push_back(temp);
            }

            std::ofstream wyjscie_doniczki("doniczki.json");
            wyjscie_doniczki << j_zapis_doniczek.dump(4);
            wyjscie_doniczki.close();

            cout << "Zapisano baze gatunkow i stan fizycznych doniczek. Zamykanie systemu..." << endl;
            break;
        }
        else {
            cout << "Nieznana komenda." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    return 0;
}