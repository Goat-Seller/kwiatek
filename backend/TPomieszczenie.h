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

    bool addPotByName(const std::string& nazwa);
    bool deletePotByName(const std::string& nazwa);
    void showContents() const;
    void regulateTemperature(const std::vector<TPomieszczenie*>& dostepnePokoje);

    const std::string& getName() const { return nazwaPomieszczenia; }
    double getTemperature() const { return temperaturaPomieszczenia; }
    const std::vector<TDoniczka*>& getPots() const { return doniczki; }
};