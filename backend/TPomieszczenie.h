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
    TPomieszczenie(const std::string& nazwa, double tempPoczatkowa);

    bool dodajDoniczkePoNazwie(const std::string& nazwa);
    bool usunDoniczkePoNazwie(const std::string& nazwa);
    void wyswietlZawartosc() const;
    void regulujTermostat(const std::vector<TPomieszczenie*>& dostepnePokoje);

    const std::string& pobierzNazwe() const { return nazwaPomieszczenia; }
    double pobierzTemperature() const { return temperaturaPomieszczenia; }
    const std::vector<TDoniczka*>& pobierzDoniczki() const { return doniczki; }
};