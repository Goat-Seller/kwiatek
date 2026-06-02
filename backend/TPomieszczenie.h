#pragma once
#include <string>
#include <vector>
#include "TDoniczka.h"

class TPomieszczenie {
private:
    std::string nazwaPomieszczenia;
    std::vector<TDoniczka*> doniczki;
    double temperaturaPomieszczenia;
public:
    TPomieszczenie(std::string nazwa, double tempPoczatkowa);

    void dodajDoniczkePoNazwie(std::string nazwa);
    void statusPomieszczenia();
    void regulujTermostat();

    // --- NOWE FUNKCJE DLA JSONA ---
    std::string pobierzNazwe() const { return nazwaPomieszczenia; }
    double pobierzTemperature() const { return temperaturaPomieszczenia; }
    const std::vector<TDoniczka*>& pobierzDoniczki() const { return doniczki; }
};