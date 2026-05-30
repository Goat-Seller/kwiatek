#include "pch.h"
#include "BazaRoslin.h"
#include "json.hpp"
#include <fstream>
#include <ctime>

using json = nlohmann::json;

BazaRoslin::BazaRoslin(std::string sciezka) : sciezka_do_pliku(sciezka) {}

bool BazaRoslin::wczytajZPliku() {
    std::ifstream plik(sciezka_do_pliku);
    if (!plik.is_open()) return false;

    json j;
    plik >> j;
    plik.close();

    ostatnia_aktualizacja = j["system_info"]["ostatnia_aktualizacja"];
    lista_roslin.clear();
    bazaGatunkow.clear(); // Czyścimy bazę gatunków przed nowym wczytaniem

    for (const auto& r : j["rosliny"]) {
        // 1. Wczytujemy roślinę
        Roslina nowa;
        nowa.id = r["id"];
        nowa.nazwa = r["nazwa"];
        nowa.wilgotnosc_min = r["wilgotnosc_min"];
        nowa.tolerancja = r["tolerancja"];
        nowa.czas_podlewania_s = r["czas_podlewania_s"];
        nowa.ostatnie_podlanie = r.value("ostatnie_podlanie", "Brak danych");
        lista_roslin.push_back(nowa);

        // 2. Tworzymy wzorzec gatunku (jeśli go jeszcze nie ma w pamięci)
        // Zakładamy, że temp. domyślna to 22.0, bo nie ma jej w obecnym JSON
        bazaGatunkow.emplace_back(nowa.nazwa, (double)nowa.wilgotnosc_min, 22.0);
    }
    return true;
}

const TGatunek* BazaRoslin::pobierzGatunek(const std::string& nazwa) const {
    for (const auto& g : bazaGatunkow) {
        if (g.pobierzNazwe() == nazwa) return &g;
    }
    return nullptr;
}

bool BazaRoslin::zapiszDoPliku() {
    json j;
    j["system_info"]["ostatnia_aktualizacja"] = ostatnia_aktualizacja;

    for (const auto& r : lista_roslin) {
        json temp;
        temp["id"] = r.id;
        temp["nazwa"] = r.nazwa;
        temp["wilgotnosc_min"] = r.wilgotnosc_min;
        temp["tolerancja"] = r.tolerancja;
        temp["czas_podlewania_s"] = r.czas_podlewania_s;
        temp["ostatnie_podlanie"] = r.ostatnie_podlanie; // <--- ZAPISUJEMY NOWE POLE

        j["rosliny"].push_back(temp);
    }

    std::ofstream plik(sciezka_do_pliku);
    if (!plik.is_open()) return false;
    plik << j.dump(4);
    plik.close();
    return true;
}

void BazaRoslin::wyswietlWszystkie() {
    std::cout << "--- BAZA ROSLIN ---" << std::endl;
    for (const auto& r : lista_roslin) {
        std::cout << "- [" << r.id << "] " << r.nazwa << std::endl;
    }
    std::cout << "-------------------" << std::endl;
}

void BazaRoslin::sprawdzRosline(const std::string& id) {
    for (const auto& r : lista_roslin) {
        if (r.id == id) {
            std::cout << "Informacje o: " << r.nazwa << std::endl;
            std::cout << "  Min. wilgotnosc: " << r.wilgotnosc_min << "% (tolerancja +-" << r.tolerancja << "%)" << std::endl;
            std::cout << "  Czas pracy pompy: " << r.czas_podlewania_s << " s" << std::endl;
            std::cout << "  OSTATNIE PODLANIE: " << r.ostatnie_podlanie << std::endl; // <--- WYŚWIETLAMY KIEDY
            return;
        }
    }
    std::cout << "Nie znaleziono rosliny o ID: " << id << std::endl;
}

void BazaRoslin::podlej(const std::string& id) {
    // UWAGA: Zmieniliśmy na auto& (z referencją), żeby móc modyfikować roślinę na liście!
    for (auto& r : lista_roslin) {
        if (r.id == id) {
            std::cout << ">>> URUCHAMIAM POMPKE DLA: " << r.nazwa << " <<<" << std::endl;

            // Kod do pobrania aktualnego czasu komputera w Visual Studio
            auto t = std::time(nullptr);
            struct tm timeinfo;
            localtime_s(&timeinfo, &t);
            char buffer[20];
            // Formatujemy czas do postaci RRRR-MM-DD GG:MM (np. 2026-05-13 20:30)
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &timeinfo);

            r.ostatnie_podlanie = std::string(buffer); // <--- ZAPISUJEMY NOWY CZAS DO OBIEKTU

            std::cout << "Zanotowano podlewanie z data: " << r.ostatnie_podlanie << std::endl;
            return;
        }
    }
    std::cout << "Nie mozna podlac. Brak rosliny o ID: " << id << std::endl;
}