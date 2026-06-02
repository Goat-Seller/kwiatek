#include "pch.h"
#include "BazaRoslin.h"
#include "TDoniczka.h"
#include "TPomieszczenie.h"
#include "TGatunek.h"
#include <iostream>
#include <string>

void wyswietlMenu() {
    std::cout << "\n=== PANEL STEROWANIA BAZA RELACYJNA ===" << std::endl;
    std::cout << "1. dodaj_gatunek" << std::endl;
    std::cout << "2. dodaj_pokoj" << std::endl;
    std::cout << "3. dodaj_doniczke (tworzy doniczke w systemie)" << std::endl;
    std::cout << "4. przypisz_doniczke (wstawia doniczke do pokoju)" << std::endl;
    std::cout << "5. usun_z_pokoju (usuwa doniczke z pokoju)" << std::endl;
    std::cout << "6. info_pokoj (wyswietla stan pokoju)" << std::endl;
    std::cout << "7. wyjscie" << std::endl;
    std::cout << "Wpisz komende (nazwe lub numer): ";
}

int main() {
    std::cout << "Trwa ladowanie systemu..." << std::endl;

    BazaRoslin baza("baza_relacyjna.json");
    baza.wczytajZPliku();

    std::string komenda;

    while (true) {
        wyswietlMenu();
        std::cin >> komenda;

        if (komenda == "wyjscie" || komenda == "7") {
            std::cout << "Zamykanie systemu. Do zobaczenia!" << std::endl;
            break;
        }
        else if (komenda == "dodaj_gatunek" || komenda == "1") {
            std::string nazwa; double minW, tempD;
            std::cout << "Podaj nazwe gatunku: "; std::cin >> nazwa;
            std::cout << "Podaj minimalna wilgotnosc (%): "; std::cin >> minW;
            std::cout << "Podaj docelowa temperature (st C): "; std::cin >> tempD;

            baza.dodajGatunek(new TGatunek(nazwa, minW, tempD));
            std::cout << "[SUKCES] Gatunek dodany!" << std::endl;
        }
        else if (komenda == "dodaj_pokoj" || komenda == "2") {
            std::string nazwa; double temp;
            std::cout << "Podaj nazwe pokoju: "; std::cin >> nazwa;
            std::cout << "Podaj temperature poczatkowa (st C): "; std::cin >> temp;

            baza.dodajPomieszczenie(new TPomieszczenie(nazwa, temp));
            std::cout << "[SUKCES] Pokoj dodany!" << std::endl;
        }
        else if (komenda == "dodaj_doniczke" || komenda == "3") {
            std::string nazwaDoniczki, nazwaGatunku;
            std::cout << "Podaj nazwe doniczki: "; std::cin >> nazwaDoniczki;
            std::cout << "Podaj nazwe przypisanego gatunku: "; std::cin >> nazwaGatunku;

            const TGatunek* g = baza.pobierzGatunek(nazwaGatunku);
            if (g != nullptr) {
                baza.dodajDoniczke(new TDoniczka(nazwaDoniczki, g));
                std::cout << "[SUKCES] Doniczka utworzona w systemie!" << std::endl;
            }
            else {
                std::cout << "[BLAD] Brak takiego gatunku!" << std::endl;
            }
        }
        // --- NOWE KOMENDY ---
        else if (komenda == "przypisz_doniczke" || komenda == "4") {
            std::string nazwaPokoju, nazwaDoniczki;
            std::cout << "Do jakiego pokoju wstawic doniczke?: "; std::cin >> nazwaPokoju;
            std::cout << "Jaka doniczke tam wstawic?: "; std::cin >> nazwaDoniczki;

            TPomieszczenie* p = baza.pobierzPomieszczenie(nazwaPokoju);
            if (p != nullptr) {
                p->dodajDoniczkePoNazwie(nazwaDoniczki);
                baza.zapiszDoPliku(); // Zrzut do JSON
                std::cout << "[SUKCES] Akcja zapisana w bazie!" << std::endl;
            }
            else {
                std::cout << "[BLAD] Nie ma takiego pokoju w systemie!" << std::endl;
            }
        }
        else if (komenda == "usun_z_pokoju" || komenda == "5") {
            std::string nazwaPokoju, nazwaDoniczki;
            std::cout << "Z jakiego pokoju usunac?: "; std::cin >> nazwaPokoju;
            std::cout << "Jaka doniczke usunac?: "; std::cin >> nazwaDoniczki;

            TPomieszczenie* p = baza.pobierzPomieszczenie(nazwaPokoju);
            if (p != nullptr) {
                if (p->usunDoniczkePoNazwie(nazwaDoniczki)) {
                    baza.zapiszDoPliku(); // Aktualizacja JSON
                    std::cout << "[SUKCES] Doniczka zniknela z pokoju i pliku!" << std::endl;
                }
                else {
                    std::cout << "[BLAD] W tym pokoju nie stoi taka doniczka!" << std::endl;
                }
            }
            else {
                std::cout << "[BLAD] Nie ma takiego pokoju w systemie!" << std::endl;
            }
        }
        else if (komenda == "info_pokoj" || komenda == "6") {
            std::string nazwaPokoju;
            std::cout << "Dla jakiego pokoju wygenerowac raport?: "; std::cin >> nazwaPokoju;

            TPomieszczenie* p = baza.pobierzPomieszczenie(nazwaPokoju);
            if (p != nullptr) {
                p->wyswietlZawartosc();
            }
            else {
                std::cout << "[BLAD] Nie ma takiego pokoju w systemie!" << std::endl;
            }
        }
        else {
            std::cout << "[BLAD] Nieznana komenda." << std::endl;
        }
    }

    return 0;
}