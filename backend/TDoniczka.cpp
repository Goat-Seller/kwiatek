#include "pch.h"
#include <iostream>
#include <random>
#include "TDoniczka.h"

using namespace std;

// Inicjalizacja statycznego rejestru
vector<TDoniczka*> TDoniczka::rejestrDoniczek;

TDoniczka::TDoniczka(string nazwaDoniczki, string nazwaGatunku)
    : nazwaDoniczki(nazwaDoniczki),
    roslinka(TGatunek::znajdzPoNazwie(nazwaGatunku))
{
    aktualizujWilgotnosc();
    // Każda nowa doniczka dopisuje się do globalnej listy
    rejestrDoniczek.push_back(this);
}

void TDoniczka::aktualizujWilgotnosc() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> rozkladWilgotnosc(10.0, 90.0);
    aktualnaWilgotnosc = rozkladWilgotnosc(gen);
}

// Funkcja wyszukująca doniczkę po nazwie w rejestrze
TDoniczka* TDoniczka::znajdzDoniczke(string nazwa) {
    for (auto d : rejestrDoniczek) {
        if (d->nazwaDoniczki == nazwa) return d;
    }
    return nullptr; // Nie znaleziono
}

void TDoniczka::StatusDoniczkiX(double tempOtoczenia) const {
    cout << "\n=== STATUS DONICZKI: " << nazwaDoniczki << " ===" << endl;
    cout << "Zasadzona roslina:    " << roslinka.pobierzNazwe() << endl;
    cout << "Aktualna wilgotnosc:  " << aktualnaWilgotnosc << " %" << endl;
    // Pobiera temperaturę przekazaną przez pomieszczenie
    cout << "Temperatura otoczenia: " << tempOtoczenia << " st. C (Optymalna: " << roslinka.pobierzTemperature() << " st. C)" << endl;
    cout << "====================================\n";
}

void TDoniczka::Podlewanie() {
    double optymalna = roslinka.pobierzMinWilgotnosc();
    if (aktualnaWilgotnosc < optymalna) {
        cout << "-> [" << nazwaDoniczki << "] Podlewanie..." << endl;
        aktualnaWilgotnosc = optymalna;
    }
}

void TDoniczka::ZmianaTemperatury(double tempOtoczenia) {
    double optymalna = roslinka.pobierzTemperature();
    cout << "-> [" << nazwaDoniczki << "] Weryfikacja klimatu (Temp otoczenia: " << tempOtoczenia << " st. C)..." << endl;

    if (tempOtoczenia < optymalna) cout << "   UWAGA: Roslinie jest za zimno w tym pokoju!" << endl;
    else if (tempOtoczenia > optymalna) cout << "   UWAGA: Roslinie jest za cieplo w tym pokoju!" << endl;
    else cout << "   Klimat idealny." << endl;
}