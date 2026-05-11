#include <iostream>
#include <random>
#include "TDoniczka.h"

using namespace std;

TDoniczka::TDoniczka(std::string nazwaDoniczki, std::string nazwaGatunku) // Inicjalizacja nazwy doniczki i obiektu rośliny na podstawie nazwy gatunku
    : nazwaDoniczki(nazwaDoniczki),
	roslinka(TGatunek::znajdzPoNazwie(nazwaGatunku))// Inicjalizacja obiektu rośliny na podstawie nazwy gatunku
{
    aktualizujWilgotnosc(); // przpisanie wilgotnosci
    aktualizujTemperatura(); // przypisanie temperatury
}

// Metoda do aktualizacji wilgotności - symulacja odczytu z czujnika
void TDoniczka::aktualizujWilgotnosc() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> rozkladWilgotnosc(10.0, 90.0);

    aktualnaWilgotnosc = rozkladWilgotnosc(gen);
}

// Metoda do aktualizacji temperatury - symulacja odczytu z czujnika
void TDoniczka::aktualizujTemperatura() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> rozkladTemperatura(15.0, 30.0);

    aktualnaTemperatura = rozkladTemperatura(gen);
}

void TDoniczka::StatusDoniczkiX() const {
    cout << "\n=== STATUS DONICZKI: " << nazwaDoniczki << " ===" << endl;
    cout << "Zasadzona roslina:    " << roslinka.pobierzNazwe() << endl;
    cout << "Aktualna wilgotnosc:  " << aktualnaWilgotnosc << " % (Wymagane min: " << roslinka.pobierzMinWilgotnosc() << " %)" << endl;
    cout << "Aktualna temperatura: " << aktualnaTemperatura << " st. C (Optymalna: " << roslinka.pobierzTemperature() << " st. C)" << endl;
    cout << "====================================\n";
}

// Metoda do podlewania rośliny, która jest wywoływana, gdy wilgotność jest poniżej wymaganego minimum lub powyżej maksimum
void TDoniczka::Podlewanie() {
    cout << "Sprawdzanie poziomu wody w: " << nazwaDoniczki << "..." << endl;

    double optymalnaWilgotnosc = roslinka.pobierzMinWilgotnosc();
    if (aktualnaWilgotnosc < optymalnaWilgotnosc) {
        double roznica = optymalnaWilgotnosc - aktualnaWilgotnosc;
        cout << "-> Roslina ma za sucho! Podlewanie..." << endl;
        aktualnaWilgotnosc = optymalnaWilgotnosc;
        cout << "-> Zwiekszono wilgotnosc o " << roznica << " % do poziomu: " << aktualnaWilgotnosc << " %\n";
    }
    else if (aktualnaWilgotnosc > optymalnaWilgotnosc) {
        double roznica = aktualnaWilgotnosc - optymalnaWilgotnosc;
        cout << "-> Roslina ma za duzo wody! Nie podlewamy." << endl;
    }
    else {
        cout << "-> Wilgotnosc jest idealna (" << aktualnaWilgotnosc << " %). Brak zmian.\n";
    }
}

// Metoda do zmiany temperatury, która jest wywoływana, gdy temperatura jest inna niż docelowa temperatura wymagana przez gatunek
void TDoniczka::ZmianaTemperatury() {
    cout << "Sprawdzanie temperatury w: " << nazwaDoniczki << "..." << endl;

    double optymalnaTemperatura = roslinka.pobierzTemperature();

    if (aktualnaTemperatura < optymalnaTemperatura) {
        double roznica = optymalnaTemperatura - aktualnaTemperatura;
        cout << "-> Jest za zimno! Uruchamianie podgrzewania..." << endl;
        aktualnaTemperatura = optymalnaTemperatura;
        cout << "-> Podwyzszono temperature o " << roznica << " st. C do: " << aktualnaTemperatura << " st. C\n";
    }
    else if (aktualnaTemperatura > optymalnaTemperatura) {
        double roznica = aktualnaTemperatura - optymalnaTemperatura;
        cout << "-> Jest za goraco! Uruchamianie chlodzenia..." << endl;
        aktualnaTemperatura = optymalnaTemperatura;
        cout << "-> Obnizono temperature o " << roznica << " st. C do: " << aktualnaTemperatura << " st. C\n";
    }
    else {
        cout << "-> Temperatura jest idealna (" << aktualnaTemperatura << " st. C). Brak zmian.\n";
    }
}