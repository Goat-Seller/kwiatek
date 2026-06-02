#include "pch.h"
#include "BazaRoslin.h"
#include "json.hpp"
#include <fstream>
#include <ctime>

using json = nlohmann::json;

BazaRoslin::BazaRoslin(std::string sciezka) : sciezka_do_pliku(sciezka) {}

// --- NOWY ODCZYT JSON (Zadanie 1) ---
bool BazaRoslin::wczytajZPliku() {
    std::ifstream plik(sciezka_do_pliku);
    if (!plik.is_open()) return false;

    json j;
    plik >> j;
    plik.close();

    // Zabezpieczenie przed brakiem pola
    ostatnia_aktualizacja = j.contains("system_info") ? j["system_info"].value("ostatnia_aktualizacja", "Brak") : "Brak danych";

    bazaGatunkow.clear();
    listaDoniczek.clear();
    listaPomieszczen.clear();

    // 1. Wczytujemy Gatunki
    if (j.contains("gatunki")) {
        for (const auto& g : j["gatunki"]) {
            bazaGatunkow.emplace_back(g["nazwa"], g["min_wilgotnosc"], g["temp_docelowa"]);
        }
    }

    // 2. Wczytujemy Doniczki
    if (j.contains("doniczki")) {
        for (const auto& d : j["doniczki"]) {
            std::string nazwaGatunku = d["gatunek"];
            const TGatunek* wskaznikNaGatunek = pobierzGatunek(nazwaGatunku);

            TDoniczka nowaDoniczka(d["nazwa_doniczki"], wskaznikNaGatunek);
            nowaDoniczka.ustawOstatniePodlanie(d.value("ostatnie_podlanie", "Brak danych"));
            listaDoniczek.push_back(nowaDoniczka);
        }
    }

    // 3. Wczytujemy Pomieszczenia
    if (j.contains("pomieszczenia")) {
        for (const auto& p : j["pomieszczenia"]) {
            TPomieszczenie nowePomieszczenie(p["nazwa_pomieszczenia"], p["temperatura"]);
            // Dodajemy wskaźniki na doniczki do pokoju
            for (const auto& nazwaDon : p["doniczki"]) {
                nowePomieszczenie.dodajDoniczkePoNazwie(nazwaDon);
            }
            listaPomieszczen.push_back(nowePomieszczenie);
        }
    }
    return true;
}

// --- NOWY ZAPIS JSON (Zadanie 1 i 2) ---
bool BazaRoslin::zapiszDoPliku() {
    json j;
    j["system_info"]["ostatnia_aktualizacja"] = ostatnia_aktualizacja;

    // 1. Zapis Gatunków
    for (const auto& g : bazaGatunkow) {
        json temp;
        temp["nazwa"] = g.pobierzNazwe();
        temp["min_wilgotnosc"] = g.pobierzMinWilgotnosc();
        temp["temp_docelowa"] = g.pobierzTemperature();
        j["gatunki"].push_back(temp);
    }

    // 2. Zapis Doniczek
    for (const auto& d : listaDoniczek) {
        json temp;
        temp["nazwa_doniczki"] = d.pobierzNazweDoniczki();
        // Jeśli wskaźnik istnieje, pobieramy nazwę, w przeciwnym razie wpisujemy Brak
        temp["gatunek"] = d.pobierzGatunek() ? d.pobierzGatunek()->pobierzNazwe() : "Brak przypisanego gatunku";
        temp["ostatnie_podlanie"] = d.pobierzOstatniePodlanie();
        j["doniczki"].push_back(temp);
    }

    // 3. Zapis Pomieszczeń
    for (const auto& p : listaPomieszczen) {
        json temp;
        temp["nazwa_pomieszczenia"] = p.pobierzNazwe();
        temp["temperatura"] = p.pobierzTemperature();
        // Wyciągamy nazwy doniczek z wskaźników
        for (const auto& d : p.pobierzDoniczki()) {
            if (d != nullptr) {
                temp["doniczki"].push_back(d->pobierzNazweDoniczki());
            }
        }
        j["pomieszczenia"].push_back(temp);
    }

    std::ofstream plik(sciezka_do_pliku);
    if (!plik.is_open()) return false;
    plik << j.dump(4);
    plik.close();
    return true;
}

// --- ZADANIE 2: FUNKCJE DODAJĄCE DO BAZY ---
void BazaRoslin::dodajGatunek(const TGatunek& nowyGatunek) {
    bazaGatunkow.push_back(nowyGatunek);
    zapiszDoPliku(); // Zrzut do JSON natychmiast po zmianie
}

void BazaRoslin::dodajDoniczke(const TDoniczka& nowaDoniczka) {
    listaDoniczek.push_back(nowaDoniczka);
    zapiszDoPliku();
}

void BazaRoslin::dodajPomieszczenie(const TPomieszczenie& nowePomieszczenie) {
    listaPomieszczen.push_back(nowePomieszczenie);
    zapiszDoPliku();
}

const TGatunek* BazaRoslin::pobierzGatunek(const std::string& nazwa) const {
    for (const auto& g : bazaGatunkow) {
        if (g.pobierzNazwe() == nazwa) return &g;
    }
    return nullptr;
}


// ==========================================================
// STARE METODY (Zostawiamy je tymczasowo jako puste atrapy, 
// żeby Twój stary Main.cpp z obrazka się w ogóle skompilował)
// ==========================================================
void BazaRoslin::wyswietlWszystkie() {
    std::cout << "Przechodzimy na nowy system relacyjny. Zaktualizuj komendy w Main!" << std::endl;
}
void BazaRoslin::sprawdzRosline(const std::string& id) {
    std::cout << "Komenda info jest aktualizowana..." << std::endl;
}
void BazaRoslin::podlej(const std::string& id) {
    std::cout << "Komenda podlej jest aktualizowana..." << std::endl;
}

TDoniczka* TDoniczka::znajdzDoniczke(std::string nazwa) {
    for (TDoniczka* d : rejestrDoniczek) {
        if (d != nullptr && d->pobierzNazweDoniczki() == nazwa) {
            return d; // Znaleziono doniczkę, zwracamy wskaźnik
        }
    }
    return nullptr; // Nic nie znaleziono
}