#include "pch.h"
#include "TGatunek.h"

TGatunek::TGatunek(std::string nazwa, double minWilg, double temp) {
    this->nazwaGatunku = nazwa;
    this->minWilgotnosc = minWilg;
    this->docelowaTemperatura = temp;
}