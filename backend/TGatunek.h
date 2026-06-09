#pragma once
#include <string>

class TGatunek {
private:
    std::string nazwaGatunku;
    double minWilgotnosc;
    double docelowaTemperatura;

public:
    TGatunek(const std::string& nazwa, double minWilg, double temp);

    const std::string& getName() const { return nazwaGatunku; }
    double getMinHumidity() const { return minWilgotnosc; }
    double getTemperature() const { return docelowaTemperatura; }

    void setMinHumidity(double h) { minWilgotnosc = h; }
    void setTemperature(double t) { docelowaTemperatura = t; }
};