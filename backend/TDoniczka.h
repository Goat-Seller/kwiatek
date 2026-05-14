#pragma once
#include <string>
#include <vector>
#include "TGatunek.h"

class TDoniczka {
private:
	std::string nazwaDoniczki;
	TGatunek roslinka;
	double aktualnaWilgotnosc;

	static std::vector<TDoniczka*> rejestrDoniczek;

public:
	TDoniczka(std::string nazwaDoniczki, std::string nazwaGatunku);

	void aktualizujWilgotnosc();

	void StatusDoniczkiX(double tempOtoczenia) const;
	void Podlewanie();
	void ZmianaTemperatury(double tempOtoczenia);

	std::string pobierzNazweDoniczki() const { return nazwaDoniczki; }

	static TDoniczka* znajdzDoniczke(std::string nazwa);
};