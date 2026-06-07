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

    void aktualizujWilgotnosc();
    void StatusDoniczkiX(double tempOtoczenia) const;
    bool Podlewanie();

    std::string pobierzNazweDoniczki() const { return nazwaDoniczki; }
    const TGatunek* pobierzGatunek() const { return roslinka; }
    std::string pobierzOstatniePodlanie() const { return ostatnie_podlanie; }
    double pobierzWilgotnosc() const { return aktualnaWilgotnosc; }

    void ustawOstatniePodlanie(const std::string& data) { ostatnie_podlanie = data; }
    void ustawWilgotnosc(double w) { aktualnaWilgotnosc = w; }

    static TDoniczka* znajdzDoniczke(const std::string& nazwa);
};