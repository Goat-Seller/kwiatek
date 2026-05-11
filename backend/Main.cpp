#include <iostream>
#include "TDoniczka.h"

using namespace std;

int main() {
    cout << "Uruchamianie systemu nawadniania..." << endl << endl;

    TDoniczka doniczka1("Doniczka Marka", "Kaktus");

    doniczka1.StatusDoniczkiX();
	doniczka1.Podlewanie();
	doniczka1.ZmianaTemperatury();
    return 0;
}