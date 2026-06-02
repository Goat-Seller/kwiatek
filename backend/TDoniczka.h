#pragma once
#include <string>
#include <vector>
#include "TGatunek.h"

class TDoniczka {
private:
    std::string nazwaDoniczki;
    const TGatunek* roslinka;
    double aktualnaWilgotnosc;

    // NOWE POLE (inicjalizowane domyślnie, nie psuje starego kodu)
    std::string ostatnie_podlanie = "Brak danych";

    static std::vector<TDoniczka*> rejestrDoniczek;
public:
    TDoniczka(std::string nazwaDoniczki, const TGatunek* wzorzecGatunku);

    void aktualizujWilgotnosc();
    void StatusDoniczkiX(double tempOtoczenia) const;
    void Podlewanie();
    void ZmianaTemperatury(double tempOtoczenia);

    std::string pobierzNazweDoniczki() const { return nazwaDoniczki; }

    // --- NOWE FUNKCJE DLA JSONA ---
    const TGatunek* pobierzGatunek() const { return roslinka; }
    std::string pobierzOstatniePodlanie() const { return ostatnie_podlanie; }
    void ustawOstatniePodlanie(std::string data) { ostatnie_podlanie = data; }

    static TDoniczka* znajdzDoniczke(std::string nazwa);
};