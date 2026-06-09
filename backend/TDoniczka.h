#pragma once
#include <string>
#include <vector>
#include "TGatunek.h"

class TDoniczka {
private:
    std::string nazwaDoniczki;
    const TGatunek* roslinka;
    double aktualnaWilgotnosc;
    std::string ostatnie_podlanie = "Brak danych";

    static std::vector<TDoniczka*> rejestrDoniczek;

public:
    TDoniczka(const std::string& nazwa, const TGatunek* wzorzecGatunku);
    ~TDoniczka();

    void setRandomHumidity();
    void StatusDoniczkiX(double tempOtoczenia) const;
    bool Podlewanie();

    const std::string& getPotName() const { return nazwaDoniczki; }
    const std::string& getLastWatering() const { return ostatnie_podlanie; }
    const TGatunek* getKind() const { return roslinka; }
    double getHumidity() const { return aktualnaWilgotnosc; }

    void setLastWatering(const std::string& data) { ostatnie_podlanie = data; }
    void setHumidity(double w) { aktualnaWilgotnosc = w; }

    static TDoniczka* getPot(const std::string& nazwa);
};