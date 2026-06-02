#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "TGatunek.h"
#include "TDoniczka.h"
#include "TPomieszczenie.h"

// Stara struktura
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

    std::vector<Roslina> lista_roslin;

    // --- NOWE RELACYJNE LISTY (Na wskaźnikach) ---
    std::vector<TGatunek*> bazaGatunkow;
    std::vector<TDoniczka*> listaDoniczek;
    std::vector<TPomieszczenie*> listaPomieszczen;

public:
    TPomieszczenie* pobierzPomieszczenie(const std::string& nazwa) const;
    BazaRoslin(std::string sciezka);
    ~BazaRoslin(); // Destruktor sprzątający pamięć

    bool wczytajZPliku();
    bool zapiszDoPliku();

    void dodajGatunek(TGatunek* nowyGatunek);
    void dodajDoniczke(TDoniczka* nowaDoniczka);
    void dodajPomieszczenie(TPomieszczenie* nowePomieszczenie);

    void wyswietlWszystkie();
    void sprawdzRosline(const std::string& id);
    void podlej(const std::string& id);

    const TGatunek* pobierzGatunek(const std::string& nazwa) const;
};