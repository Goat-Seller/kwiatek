#pragma once
#include <string>

class TGatunek {
private:
    std::string nazwaGatunku;
    double minWilgotnosc;
    double docelowaTemperatura;

public:
    TGatunek(std::string nazwa, double minWilg, double temp);

    std::string pobierzNazwe() const { return nazwaGatunku; }
    double pobierzMinWilgotnosc() const { return minWilgotnosc; }
    double pobierzTemperature() const { return docelowaTemperatura; }
};