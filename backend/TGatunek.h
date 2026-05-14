#pragma once
#include <string>
#include <vector>

class TGatunek {
private:
	std::string nazwaGatunku; // Nazwa gatunku rośliny
	double minWilgotnosc; // Minimalna wilgotność wymagana przez gatunek
	double docelowaTemperatura; // Docelowa temperatura wymagana przez gatunek

public:
	TGatunek(std::string nazwa, double minWilg, double temp); // Konstruktor

	std::string pobierzNazwe() const; // Zwraca nazwę gatunku
	double pobierzMinWilgotnosc() const; // Zwraca minimalną wilgotność wymaganą przez gatunek
	double pobierzTemperature() const; // Zwraca docelową temperaturę wymaganą przez gatunek

	static const std::vector<TGatunek>& BazaGatunkow();// Baza gatunków roślin (statyczna metoda)
	static TGatunek znajdzPoNazwie(std::string nazwa); // Znajduje gatunek po nazwie w bazie
};