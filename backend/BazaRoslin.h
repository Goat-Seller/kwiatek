#pragma once
#include <iostream>
#include <string>
#include <vector>

// Struktura przechowująca dane pojedynczej rośliny
struct Roslina {
    std::string id;
    std::string nazwa;
    int wilgotnosc_min;
    int tolerancja;
    int czas_podlewania_s;
    std::string ostatnie_podlanie;
};

class BazaRoslin {
private:
    std::vector<Roslina> lista_roslin;
    std::string sciezka_do_pliku;
    std::string ostatnia_aktualizacja;

public:
    // Konstruktor przyjmujący ścieżkę do pliku JSON
    BazaRoslin(std::string sciezka);

    // Funkcje do odczytu i zapisu
    bool wczytajZPliku();
    bool zapiszDoPliku();

    // Funkcje do obsługi z komend
    void wyswietlWszystkie();
    void sprawdzRosline(const std::string& id);
    void podlej(const std::string& id);
};