#include "pch.h"
#include <iostream>
#include <random>
#include "TDoniczka.h"

std::vector<TDoniczka*> TDoniczka::rejestrDoniczek;

TDoniczka::TDoniczka(std::string nazwaDoniczki, const TGatunek* wzorzecGatunku)
    : nazwaDoniczki(nazwaDoniczki), roslinka(wzorzecGatunku)
{
    aktualizujWilgotnosc();
    rejestrDoniczek.push_back(this);
}

void TDoniczka::aktualizujWilgotnosc() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> rozkladWilgotnosc(10.0, 90.0);
    aktualnaWilgotnosc = rozkladWilgotnosc(gen);
}

void TDoniczka::StatusDoniczkiX(double tempOtoczenia) const {
    if (!roslinka) return; // Zabezpieczenie przed brakiem danych
    std::cout << "\n=== STATUS DONICZKI: " << nazwaDoniczki << " ===" << std::endl;
    std::cout << "Zasadzona roslina:    " << roslinka->pobierzNazwe() << std::endl;
    std::cout << "Aktualna wilgotnosc:  " << aktualnaWilgotnosc << " %" << std::endl;
    std::cout << "Temperatura otoczenia: " << tempOtoczenia << " st. C (Optymalna: " << roslinka->pobierzTemperature() << " st. C)" << std::endl;
    std::cout << "====================================\n";
}

void TDoniczka::Podlewanie() {
    if (!roslinka) return;
    double optymalna = roslinka->pobierzMinWilgotnosc();
    if (aktualnaWilgotnosc < optymalna) {
        std::cout << "-> [" << nazwaDoniczki << "] Podlewanie..." << std::endl;
        aktualnaWilgotnosc = optymalna;
    }
}

void TDoniczka::ZmianaTemperatury(double tempOtoczenia) {
    if (!roslinka) return; // To jest kluczowe zabezpieczenie!

    double optymalna = roslinka->pobierzTemperature(); // Używamy -> bo roslinka to wskaźnik
    std::cout << "-> [" << nazwaDoniczki << "] Weryfikacja klimatu (Temp otoczenia: " << tempOtoczenia << " st. C)..." << std::endl;

    if (tempOtoczenia < optymalna) {
        std::cout << "   UWAGA: Roslinie jest za zimno w tym pokoju!" << std::endl;
    }
    else if (tempOtoczenia > optymalna) {
        std::cout << "   UWAGA: Roslinie jest za cieplo w tym pokoju!" << std::endl;
    }
    else {
        std::cout << "   Klimat idealny." << std::endl;
    }
}