#include "pch.h"
#include "TGatunek.h"

TGatunek::TGatunek(const std::string& nazwa, double minWilg, double temp)
    : nazwaGatunku(nazwa), minWilgotnosc(minWilg), docelowaTemperatura(temp) {
}