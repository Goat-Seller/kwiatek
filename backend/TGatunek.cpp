#include "pch.h"
#include "TGatunek.h"
#include <iostream>

using namespace std;

TGatunek::TGatunek(string nazwa, double minWilg, double temp) {
    this->nazwaGatunku = nazwa;
    this->minWilgotnosc = minWilg;
    this->docelowaTemperatura = temp;
}

string TGatunek::pobierzNazwe() const { return nazwaGatunku; } // Zwraca nazwę gatunku
double TGatunek::pobierzMinWilgotnosc() const { return minWilgotnosc; } // Zwraca minimalną wilgotność wymaganą przez gatunek
double TGatunek::pobierzTemperature() const { return docelowaTemperatura; } // Zwraca docelową temperaturę wymaganą przez gatunek