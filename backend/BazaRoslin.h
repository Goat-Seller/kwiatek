#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "TGatunek.h"
#include "TDoniczka.h"        // NOWE
#include "TPomieszczenie.h"   // NOWE

// Stara struktura - ZOSTAWIAMY NA CHWILĘ, żeby nie wywaliło błędów w .cpp
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
    std::string sciezka_do_pliku;
    std::string ostatnia_aktualizacja;

    // --- STARA LISTA (do usunięcia w kolejnym kroku) ---
    std::vector<Roslina> lista_roslin;

    // --- NOWE RELACYJNE LISTY (Nasza nowa baza danych z zadania 1) ---
    std::vector<TGatunek> bazaGatunkow;
    std::vector<TDoniczka> listaDoniczek;
    std::vector<TPomieszczenie> listaPomieszczen;

public:
    // Konstruktor
    BazaRoslin(std::string sciezka);

    // Funkcje do odczytu i zapisu JSON
    bool wczytajZPliku();
    bool zapiszDoPliku();

    // --- NOWE FUNKCJE DO DODAWANIA (Zadanie 2 z Twojego screena) ---
    void dodajGatunek(const TGatunek& nowyGatunek);
    void dodajDoniczke(const TDoniczka& nowaDoniczka);
    void dodajPomieszczenie(const TPomieszczenie& nowePomieszczenie);

    // Stare funkcje (na razie zostawiamy, żeby kompilator nie krzyczał)
    void wyswietlWszystkie();
    void sprawdzRosline(const std::string& id);
    void podlej(const std::string& id);

    const TGatunek* pobierzGatunek(const std::string& nazwa) const;
};