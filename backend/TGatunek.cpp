#include "TGatunek.h"
#include <iostream>

using namespace std;

TGatunek::TGatunek(string nazwa, double minWilg, double temp) {
    this->nazwaGatunku = nazwa;
    this->minWilgotnosc = minWilg;
    this->docelowaTemperatura = temp;
}

string TGatunek::pobierzNazwe() const { return nazwaGatunku; } // Zwraca nazwę gatunku
double TGatunek::pobierzMinWilgotnosc() const { return minWilgotnosc; } // Zwraca minimalną wilgotność wymaganą przez gatunek
double TGatunek::pobierzTemperature() const { return docelowaTemperatura; } // Zwraca docelową temperaturę wymaganą przez gatunek

const vector<TGatunek>& TGatunek::BazaGatunkow() { // Zwraca referencję do statycznej bazy gatunków
    static vector<TGatunek> baza = {
		//Nazwa gatunku, Minimalna wilgotność, Docelowa temperatura
        TGatunek{"Kaktus", 20.0, 25.0},
        TGatunek{"Paproc", 60.0, 22.0},
        TGatunek{"Monstera", 40.0, 23.0},
        TGatunek{"Fikus", 50.0, 21.0}
    };
    return baza;
}

TGatunek TGatunek::znajdzPoNazwie(string nazwa) { // Szuka gatunku po nazwie w bazie
    for (const auto& roslina : BazaGatunkow()) {
        if (roslina.pobierzNazwe() == nazwa) {
            return roslina;
        }
    }
    cout << "[UWAGA] Nie mozna poprawnie dodac doniczki z gatunkiem '" << nazwa << "', bo ten gatunek nie jest obslugiwany!" << endl;
	return TGatunek{ "NIEZNANY GATUNEK", 0.0, 0.0 }; // Zwraca "NIEZNANY GATUNEK" jeśli nie znaleziono
}