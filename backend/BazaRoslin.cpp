#include "pch.h"
#include "BazaRoslin.h"
#include "json.hpp"
#include <fstream>
#include <ctime>   

using json = nlohmann::json;

BazaRoslin::BazaRoslin(std::string sciezka) {
    sciezka_do_pliku = sciezka;
}

bool BazaRoslin::wczytajZPliku() {
    std::ifstream plik(sciezka_do_pliku);
    if (!plik.is_open()) return false;

    json j;
    plik >> j;
    plik.close();

    ostatnia_aktualizacja = j["system_info"]["ostatnia_aktualizacja"];
    lista_roslin.clear();

    for (const auto& r : j["rosliny"]) {
        Roslina nowa;
        nowa.id = r["id"];
        nowa.nazwa = r["nazwa"];
        nowa.wilgotnosc_min = r["wilgotnosc_min"];
        nowa.tolerancja = r["tolerancja"];
        nowa.czas_podlewania_s = r["czas_podlewania_s"];
        nowa.ostatnie_podlanie = r.value("ostatnie_podlanie", "Brak danych");

        lista_roslin.push_back(nowa);
    }
    return true;
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
        temp["ostatnie_podlanie"] = r.ostatnie_podlanie;

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
            std::cout << "  OSTATNIE PODLANIE: " << r.ostatnie_podlanie << std::endl;
            return;
        }
    }
    std::cout << "Nie znaleziono rosliny o ID: " << id << std::endl;
}

void BazaRoslin::podlej(const std::string& id_lub_nazwa) {
    for (auto& r : lista_roslin) {
        // Sprawdzamy czy zgadza sie ID albo NAZWA!
        if (r.id == id_lub_nazwa || r.nazwa == id_lub_nazwa) {

            auto t = std::time(nullptr);
            struct tm timeinfo;
            localtime_s(&timeinfo, &t);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &timeinfo);

            r.ostatnie_podlanie = std::string(buffer);

            std::cout << "-> Baza JSON: Zanotowano podlewanie (" << r.nazwa << ") z data: " << r.ostatnie_podlanie << std::endl;
            return;
        }
    }
    std::cout << "Baza JSON: Nie mozna zaktualizowac daty. Brak rosliny o ID/Nazwie: " << id_lub_nazwa << std::endl;
}

std::string BazaRoslin::sprawdzGatunek(const std::string& szukana_fraza) {
    for (const auto& r : lista_roslin) {
        if (r.id == szukana_fraza || r.nazwa == szukana_fraza) {
            return r.nazwa; // Zwraca prawdziwą nazwę z bazy
        }
    }
    return "BRAK"; // Zwraca flagę błędu, jeśli nie znajdzie
}

bool BazaRoslin::pobierzDaneRosliny(const std::string& szukana_fraza, Roslina& wynik) {
    for (const auto& r : lista_roslin) {
        if (r.id == szukana_fraza || r.nazwa == szukana_fraza) {
            wynik = r; // Znaleziono, kopiujemy cala paczke danych
            return true;
        }
    }
    return false; // Nie znaleziono w JSON-ie
}