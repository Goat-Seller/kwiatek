#pragma once
#include <string>
#include "TGatunek.h"

class TDoniczka {
private:
	std::string nazwaDoniczki;// Nazwa doniczki, np. "Doniczka 1"
	TGatunek roslinka; // Obiekt gatunku rośliny, który jest w doniczce
	double aktualnaWilgotnosc; // Aktualna wilgotność odczytana z czujnika (w procentach)
	double aktualnaTemperatura; // Aktualna temperatura odczytana z czujnika (w stopniach Celsjusza)

public:
	TDoniczka(std::string nazwaDoniczki, std::string nazwaGatunku, double minWilg, double temp);

	void aktualizujWilgotnosc();// Aktualizuje aktualną wilgotność odczytaną z czujnika
	void aktualizujTemperatura();// Aktualizuje aktualną temperaturę odczytaną z czujnika

    void StatusDoniczkiX() const;
	//static void StatusWszystkichDoniczek(const std::vector<TDoniczka>& listaDoniczek); // Statyczna metoda do wyświetlania statusu wszystkich doniczek

	void Podlewanie(); // Metoda do podlewania rośliny, która jest wywoływana, gdy wilgotność jest poniżej wymaganego minimum
	void ZmianaTemperatury(); // Metoda do zmiany temperatury, która jest wywoływana, gdy temperatura jest inna niż docelowa temperatura wymagana przez gatunek
	std::string pobierzNazwe() const { return nazwaDoniczki; }
	std::string pobierzGatunek() const { return roslinka.pobierzNazwe(); }
};