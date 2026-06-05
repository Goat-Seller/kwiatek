#include "pch.h"
#include "TPomieszczenie.h"
#include <iostream>
#include <iomanip>
#include <cmath>

TPomieszczenie::TPomieszczenie(const std::string& nazwa, double tempPoczatkowa)
    : nazwaPomieszczenia(nazwa), temperaturaPomieszczenia(tempPoczatkowa) {
}

bool TPomieszczenie::dodajDoniczkePoNazwie(const std::string& nazwa) {
    TDoniczka* znaleziona = TDoniczka::znajdzDoniczke(nazwa);

    if (znaleziona != nullptr) {
        doniczki.push_back(znaleziona);
        return true;
    }
    return false;
}

bool TPomieszczenie::usunDoniczkePoNazwie(const std::string& nazwa) {
    for (auto it = doniczki.begin(); it != doniczki.end(); ++it) {
        if ((*it) != nullptr && (*it)->pobierzNazweDoniczki() == nazwa) {
            doniczki.erase(it);
            return true;
        }
    }
    return false;
}

void TPomieszczenie::wyswietlZawartosc() const {
    std::cout << "\nRAPORT POKOJU: " << nazwaPomieszczenia << " ---" << std::endl;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Aktualna temperatura: " << temperaturaPomieszczenia << " st. C" << std::endl;
    std::cout << std::defaultfloat;

    std::cout << "Liczba doniczek: " << doniczki.size() << std::endl;

    if (doniczki.empty()) {
        std::cout << "  (Pokoj jest pusty)" << std::endl;
    }
    else {
        for (auto d : doniczki) {
            if (d != nullptr) {
                std::cout << "  -> " << d->pobierzNazweDoniczki();
                if (d->pobierzGatunek()) {
                    std::cout << " (Gatunek: " << d->pobierzGatunek()->pobierzNazwe() << ")";
                }
                std::cout << std::endl;
            }
        }
    }
}

void TPomieszczenie::regulujTermostat(const std::vector<TPomieszczenie*>& dostepnePokoje) {
    if (doniczki.empty()) {
        std::cout << "[INFO] Pokoj " << nazwaPomieszczenia << " jest pusty. Brak akcji." << std::endl;
        return;
    }

    double sumaTemp = 0.0;
    int liczbaRoslin = 0;

    for (auto d : doniczki) {
        if (d != nullptr && d->pobierzGatunek() != nullptr) {
            sumaTemp += d->pobierzGatunek()->pobierzTemperature();
            liczbaRoslin++;
        }
    }

    if (liczbaRoslin == 0) return;

    double wyliczonaTemp = sumaTemp / liczbaRoslin;
    temperaturaPomieszczenia = std::round(wyliczonaTemp * 10.0) / 10.0;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "[TERMOSTAT] Skalibrowano i ustawiono temperature na: " << temperaturaPomieszczenia << " st. C." << std::endl;
    std::cout << std::defaultfloat;

    bool znalezionoKonflikt = false;
    double tolerancja = 5.0;

    for (auto d : doniczki) {
        if (d != nullptr && d->pobierzGatunek() != nullptr) {
            double tempDocelowa = d->pobierzGatunek()->pobierzTemperature();
            double uchyb = std::abs(temperaturaPomieszczenia - tempDocelowa);

            if (uchyb > tolerancja) {
                znalezionoKonflikt = true;
                std::cout << "\n[OSTRZEZENIE] Doniczka '" << d->pobierzNazweDoniczki()
                    << "' wymaga " << tempDocelowa << " st. C. Zle zniesie nowe warunki!" << std::endl;

                std::string polecanyPokoj = "";
                double najmniejszyUchybWInnym = tolerancja;

                for (auto p : dostepnePokoje) {
                    if (p != nullptr && p->pobierzNazwe() != nazwaPomieszczenia) {
                        double uchybWInnym = std::abs(p->pobierzTemperature() - tempDocelowa);

                        if (uchybWInnym <= tolerancja && uchybWInnym < najmniejszyUchybWInnym) {
                            najmniejszyUchybWInnym = uchybWInnym;
                            polecanyPokoj = p->pobierzNazwe();
                        }
                    }
                }

                if (polecanyPokoj != "") {
                    std::cout << "   -> SUGESTIA: Przenies doniczke do pokoju '" << polecanyPokoj << std::endl;
                }
                else {
                    std::cout << "   -> SUGESTIA: Brak odpowiedniego pokoju w systemie! Rozwaz dodanie nowego pokoju." << std::endl;
                }
            }
        }
    }

    if (!znalezionoKonflikt) {
        std::cout << "Warunki sa bezpieczne dla wszystkich roslin w tym pomieszczeniu." << std::endl;
    }
}