#include "pch.h"
#include "TDoniczka.h"
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <iomanip>

std::vector<TDoniczka*> TDoniczka::rejestrDoniczek;

TDoniczka::TDoniczka(const std::string& nazwa, const TGatunek* wzorzecGatunku)
    : nazwaDoniczki(nazwa), roslinka(wzorzecGatunku), aktualnaWilgotnosc(0.0) {
    setRandomHumidity();
    rejestrDoniczek.push_back(this);
}

TDoniczka::~TDoniczka() {
    for (auto it = rejestrDoniczek.begin(); it != rejestrDoniczek.end(); ++it) {
        if (*it == this) {
            rejestrDoniczek.erase(it);
            break;
        }
    }
}

void TDoniczka::setRandomHumidity() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> rozklad(0.0, 100.0);

    aktualnaWilgotnosc = std::round(rozklad(gen) * 10.0) / 10.0;
}

void TDoniczka::StatusDoniczkiX(double tempOtoczenia) const {
    if (!roslinka) return;

    std::cout << "\n=== STATUS DONICZKI: " << nazwaDoniczki << " ===" << std::endl;
    std::cout << "Zasadzona roslina:    " << roslinka->getName() << std::endl;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Aktualna wilgotnosc:  " << aktualnaWilgotnosc << " %" << std::endl;
    std::cout << "Temperatura otoczenia: " << tempOtoczenia << " st. C (Optymalna: " << roslinka->getTemperature() << " st. C)" << std::endl;
    std::cout << std::defaultfloat;
}

TDoniczka* TDoniczka::getPot(const std::string& nazwa) {
    for (TDoniczka* d : rejestrDoniczek) {
        if (d != nullptr && d->getPotName() == nazwa) {
            return d;
        }
    }
    return nullptr;
}

bool TDoniczka::Podlewanie() {
    if (!roslinka) return false;

    double optymalna = roslinka->getMinHumidity();

    if (aktualnaWilgotnosc < optymalna) {
        aktualnaWilgotnosc = optymalna;

        std::time_t czas_teraz = std::time(nullptr);
        struct tm tm_info;
        localtime_s(&tm_info, &czas_teraz);

        char bufor[100];
        if (std::strftime(bufor, sizeof(bufor), "%Y-%m-%d %H:%M:%S", &tm_info)) {
            ostatnie_podlanie = bufor;
        }
        return true;
    }
    return false;
}