#pragma once
#include <string>
#include <vector>
#include "TGatunek.h"
#include "TDoniczka.h"
#include "TPomieszczenie.h"

class BazaRoslin {
private:
    std::string sciezka_do_pliku;
    std::string ostatnia_aktualizacja;

    std::vector<TGatunek*> bazaGatunkow;
    std::vector<TDoniczka*> listaDoniczek;
    std::vector<TPomieszczenie*> listaPomieszczen;

public:
    BazaRoslin(const std::string& sciezka);
    ~BazaRoslin();

    bool wczytajZPliku();
    bool zapiszDoPliku();

    void dodajGatunek(TGatunek* nowyGatunek);
    void dodajDoniczke(TDoniczka* nowaDoniczka);
    void dodajPomieszczenie(TPomieszczenie* nowePomieszczenie);

    bool usunDoniczkeZSystemu(const std::string& nazwa);
    bool usunPomieszczenieZSystemu(const std::string& nazwa);

    bool czyDoniczkaPrzypisana(const std::string& nazwaDoniczki) const;
    void wylosujWilgotnoscStartowa();
    std::string znajdzPokojDlaDoniczki(const std::string& nazwaDoniczki) const;

    TPomieszczenie* pobierzPomieszczenie(const std::string& nazwa) const;
    const TGatunek* pobierzGatunek(const std::string& nazwa) const;
    const std::vector<TPomieszczenie*>& pobierzPokoje() const { return listaPomieszczen; }
};