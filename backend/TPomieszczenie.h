#pragma once
#include <string>
#include <vector>
#include "TDoniczka.h"

class TPomieszczenie {
private:
    std::string nazwaPomieszczenia;

    // ZMIANA: Wektor przechowuje teraz WSKAŹNIKI (TDoniczka*)
    std::vector<TDoniczka*> doniczki;

    double temperaturaPomieszczenia;

public:
    TPomieszczenie(std::string nazwa, double tempPoczatkowa);

    // ZMIANA: Nowa nazwa funkcji z poprzedniego kroku
    void dodajDoniczkePoNazwie(std::string nazwa);

    void statusPomieszczenia();

    // Jeśli miałeś wcześniej tę funkcję z regulacją termostatu, zostawiamy ją w deklaracji
    void regulujTermostat();
};