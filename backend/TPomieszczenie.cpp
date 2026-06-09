#include "pch.h"
#include "TPomieszczenie.h"
#include <iostream>
#include <iomanip>
#include <cmath>

TPomieszczenie::TPomieszczenie(const std::string& nazwa, double tempPoczatkowa)
    : nazwaPomieszczenia(nazwa), temperaturaPomieszczenia(tempPoczatkowa) {
}

bool TPomieszczenie::addPotByName(const std::string& nazwa) {
    TDoniczka* znaleziona = TDoniczka::getPot(nazwa);

    if (znaleziona != nullptr) {
        doniczki.push_back(znaleziona);
        return true;
    }
    return false;
}

bool TPomieszczenie::deletePotByName(const std::string& nazwa) {
    for (auto it = doniczki.begin(); it != doniczki.end(); ++it) {
        if ((*it) != nullptr && (*it)->getPotName() == nazwa) {
            doniczki.erase(it);
            return true;
        }
    }
    return false;
}

void TPomieszczenie::showContents() const {
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
                std::cout << "  -> " << d->getPotName();
                if (d->getKind()) {
                    std::cout << " (Gatunek: " << d->getKind()->getName() << ")";
                }
                std::cout << std::endl;
            }
        }
    }
}

void TPomieszczenie::regulateTemperature(const std::vector<TPomieszczenie*>& dostepnePokoje) {
    if (doniczki.empty()) {
        std::cout << "[INFO] Pokoj " << nazwaPomieszczenia << " jest pusty. Brak akcji." << std::endl;
        return;
    }

    double sumaTemp = 0.0;
    int liczbaRoslin = 0;

    for (auto d : doniczki) {
        if (d != nullptr && d->getKind() != nullptr) {
            sumaTemp += d->getKind()->getTemperature();
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
        if (d != nullptr && d->getKind() != nullptr) {
            double tempDocelowa = d->getKind()->getTemperature();
            double uchyb = std::abs(temperaturaPomieszczenia - tempDocelowa);

            if (uchyb > tolerancja) {
                znalezionoKonflikt = true;
                std::cout << "\n[OSTRZEZENIE] Doniczka '" << d->getPotName()
                    << "' wymaga " << tempDocelowa << " st. C. Zle zniesie nowe warunki!" << std::endl;

                std::string polecanyPokoj = "";
                double najmniejszyUchybWInnym = tolerancja;

                for (auto p : dostepnePokoje) {
                    if (p != nullptr && p->getName() != nazwaPomieszczenia) {
                        double uchybWInnym = std::abs(p->getTemperature() - tempDocelowa);

                        if (uchybWInnym <= tolerancja && uchybWInnym < najmniejszyUchybWInnym) {
                            najmniejszyUchybWInnym = uchybWInnym;
                            polecanyPokoj = p->getName();
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