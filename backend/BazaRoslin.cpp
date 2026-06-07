#include "pch.h"
#include "BazaRoslin.h"
#include "json.hpp"
#include <fstream>
#include <ctime>

using json = nlohmann::json;

BazaRoslin::BazaRoslin(const std::string& sciezka) : sciezka_do_pliku(sciezka) {}

BazaRoslin::~BazaRoslin() {
    for (auto p : listaPomieszczen) delete p;
    for (auto d : listaDoniczek) delete d;
    for (auto g : bazaGatunkow) delete g;
}

bool BazaRoslin::wczytajZPliku() {
    std::ifstream plik(sciezka_do_pliku);
    if (!plik.is_open()) return false;

    json j;
    plik >> j;
    plik.close();

    ostatnia_aktualizacja = j.contains("system_info") ? j["system_info"].value("ostatnia_aktualizacja", "Brak") : "Brak danych";

    bazaGatunkow.clear();
    listaDoniczek.clear();
    listaPomieszczen.clear();

    if (j.contains("gatunki")) {
        for (const auto& g : j["gatunki"]) {
            bazaGatunkow.push_back(new TGatunek(g["nazwa"], g["min_wilgotnosc"], g["temp_docelowa"]));
        }
    }

    if (j.contains("doniczki")) {
        for (const auto& d : j["doniczki"]) {
            const TGatunek* wskaznikNaGatunek = pobierzGatunek(d["gatunek"]);
            TDoniczka* nowaDoniczka = new TDoniczka(d["nazwa_doniczki"], wskaznikNaGatunek);
            nowaDoniczka->ustawOstatniePodlanie(d.value("ostatnie_podlanie", "Brak danych"));
            if (d.contains("aktualna_wilgotnosc")) nowaDoniczka->ustawWilgotnosc(d["aktualna_wilgotnosc"]);
            listaDoniczek.push_back(nowaDoniczka);
        }
    }

    if (j.contains("pomieszczenia")) {
        for (const auto& p : j["pomieszczenia"]) {
            TPomieszczenie* nowePomieszczenie = new TPomieszczenie(p["nazwa_pomieszczenia"], p["temperatura"]);

            if (p.contains("doniczki")) {
                for (const auto& nazwaDon : p["doniczki"]) {
                    nowePomieszczenie->dodajDoniczkePoNazwie(nazwaDon);
                }
            }
            listaPomieszczen.push_back(nowePomieszczenie);
        }
    }
    return true;
}

bool BazaRoslin::zapiszDoPliku() {
    json j;
    j["system_info"]["ostatnia_aktualizacja"] = ostatnia_aktualizacja;

    for (const auto& g : bazaGatunkow) {
        json temp;
        temp["nazwa"] = g->pobierzNazwe();
        temp["min_wilgotnosc"] = g->pobierzMinWilgotnosc();
        temp["temp_docelowa"] = g->pobierzTemperature();
        j["gatunki"].push_back(temp);
    }

    for (const auto& d : listaDoniczek) {
        json temp;
        temp["nazwa_doniczki"] = d->pobierzNazweDoniczki();
        temp["gatunek"] = d->pobierzGatunek() ? d->pobierzGatunek()->pobierzNazwe() : "Brak";
        temp["ostatnie_podlanie"] = d->pobierzOstatniePodlanie();
        temp["aktualna_wilgotnosc"] = d->pobierzWilgotnosc();
        j["doniczki"].push_back(temp);
    }

    for (const auto& p : listaPomieszczen) {
        json temp;
        temp["nazwa_pomieszczenia"] = p->pobierzNazwe();
        temp["temperatura"] = p->pobierzTemperature();
        temp["doniczki"] = json::array();

        for (const auto& d : p->pobierzDoniczki()) {
            if (d != nullptr) temp["doniczki"].push_back(d->pobierzNazweDoniczki());
        }
        j["pomieszczenia"].push_back(temp);
    }

    std::ofstream plik(sciezka_do_pliku);
    if (!plik.is_open()) return false;
    plik << j.dump(4);
    plik.close();
    return true;
}

void BazaRoslin::dodajGatunek(TGatunek* nowyGatunek) {
    bazaGatunkow.push_back(nowyGatunek);
    zapiszDoPliku();
}

void BazaRoslin::dodajDoniczke(TDoniczka* nowaDoniczka) {
    listaDoniczek.push_back(nowaDoniczka);
    zapiszDoPliku();
}

void BazaRoslin::dodajPomieszczenie(TPomieszczenie* nowePomieszczenie) {
    listaPomieszczen.push_back(nowePomieszczenie);
    zapiszDoPliku();
}

const TGatunek* BazaRoslin::pobierzGatunek(const std::string& nazwa) const {
    for (const auto& g : bazaGatunkow) {
        if (g->pobierzNazwe() == nazwa) return g;
    }
    return nullptr;
}

TPomieszczenie* BazaRoslin::pobierzPomieszczenie(const std::string& nazwa) const {
    for (auto p : listaPomieszczen) {
        if (p->pobierzNazwe() == nazwa) return p;
    }
    return nullptr;
}

bool BazaRoslin::usunDoniczkeZSystemu(const std::string& nazwa) {
    for (auto p : listaPomieszczen) {
        if (p != nullptr) {
            p->usunDoniczkePoNazwie(nazwa);
        }
    }

    for (auto it = listaDoniczek.begin(); it != listaDoniczek.end(); ++it) {
        if (*it != nullptr && (*it)->pobierzNazweDoniczki() == nazwa) {
            delete* it;
            listaDoniczek.erase(it);
            zapiszDoPliku();
            return true;
        }
    }
    return false;
}

bool BazaRoslin::usunPomieszczenieZSystemu(const std::string& nazwa) {
    for (auto it = listaPomieszczen.begin(); it != listaPomieszczen.end(); ++it) {
        if (*it != nullptr && (*it)->pobierzNazwe() == nazwa) {
            delete* it;
            listaPomieszczen.erase(it);
            zapiszDoPliku();
            return true;
        }
    }
    return false;
}

bool BazaRoslin::czyDoniczkaPrzypisana(const std::string& nazwaDoniczki) const {
    for (auto p : listaPomieszczen) {
        if (p != nullptr) {
            for (auto d : p->pobierzDoniczki()) {
                if (d != nullptr && d->pobierzNazweDoniczki() == nazwaDoniczki) {
                    return true;
                }
            }
        }
    }
    return false;
}

void BazaRoslin::wylosujWilgotnoscStartowa() {
    for (auto d : listaDoniczek) {
        if (d != nullptr) {
            d->aktualizujWilgotnosc();
        }
    }
    zapiszDoPliku();
}

std::string BazaRoslin::znajdzPokojDlaDoniczki(const std::string& nazwaDoniczki) const {
    for (auto p : listaPomieszczen) {
        if (p != nullptr) {
            for (auto d : p->pobierzDoniczki()) {
                if (d != nullptr && d->pobierzNazweDoniczki() == nazwaDoniczki) {
                    return p->pobierzNazwe();
                }
            }
        }
    }
    return "Brak";
}