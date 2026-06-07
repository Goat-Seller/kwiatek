#include "pch.h"
#include "BazaRoslin.h"
#include "TDoniczka.h"
#include "TPomieszczenie.h"
#include "TGatunek.h"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <cmath>
#include <random>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// Include your backend and wrapper
#include "TDoniczka.h"
#include "DoniczkaWrapper.h"

void wyswietlMenu() {
    std::cout << "\n=== PANEL STEROWANIA ===" << std::endl;
    std::cout << "1. dodaj_gatunek" << std::endl;
    std::cout << "2. dodaj_doniczke" << std::endl;
    std::cout << "3. usun_doniczke" << std::endl;
    std::cout << "4. status_doniczki" << std::endl;
    std::cout << "5. dodaj_pokoj" << std::endl;
    std::cout << "6. usun_pokoj" << std::endl;
    std::cout << "7. przypisz_doniczke" << std::endl;
    std::cout << "8. usun_z_pokoju" << std::endl;
    std::cout << "9. info_pokoj" << std::endl;
    std::cout << "10. podlej" << std::endl;
    std::cout << "11. reguluj_termostat" << std::endl;
    std::cout << "12. wyjscie" << std::endl;
    std::cout << "Wpisz komende (nazwe lub numer): ";
}
int main(int argc, char* argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qputenv("QT_QUICK_CONTROLS_STYLE", "Fusion");

int main() {
    BazaRoslin baza("baza_relacyjna.json");
    baza.wczytajZPliku();
    QGuiApplication app(argc, argv);


    std::string komenda;
    // 1. Create pure C++ backend object
    TDoniczka backendDoniczka("Doniczka 1", "Fikus");

    while (true) {
        wyswietlMenu();
        std::cin >> komenda;

        if (komenda == "dodaj_gatunek" || komenda == "1") {
            std::string nazwa;
            double minW, tempD;
            std::cout << "Podaj nazwe gatunku: ";
            std::cin >> nazwa;

            std::cout << "Podaj minimalna wilgotnosc (%): ";
            while (!(std::cin >> minW)) {
                std::cout << "Wartosc podajemy jako liczbe (np. 45.5)! -> ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            minW = std::round(minW * 10.0) / 10.0;

            std::cout << "Podaj docelowa temperature (st C): ";
            while (!(std::cin >> tempD)) {
                std::cout << "Wartosc podajemy jako liczbe (np. 22.3)! -> ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            tempD = std::round(tempD * 10.0) / 10.0;

            baza.dodajGatunek(new TGatunek(nazwa, minW, tempD));
            std::cout << "[SUKCES] Gatunek dodany!" << std::endl;
        }
        else if (komenda == "dodaj_doniczke" || komenda == "2") {
            std::string nazwaDoniczki, nazwaGatunku;
            std::cout << "Podaj nazwe doniczki: ";
            std::cin >> nazwaDoniczki;
            std::cout << "Podaj nazwe przypisanego gatunku: ";
            std::cin >> nazwaGatunku;

            const TGatunek* g = baza.pobierzGatunek(nazwaGatunku);
            if (g != nullptr) {
                baza.dodajDoniczke(new TDoniczka(nazwaDoniczki, g));
                std::cout << "[SUKCES] Doniczka utworzona w systemie!" << std::endl;
            }
            else {
                std::cout << "[BLAD] Brak takiego gatunku!" << std::endl;
            }
        }
        else if (komenda == "usun_doniczke" || komenda == "3") {
            std::string nazwaDoniczki;
            std::cout << "Podaj nazwe doniczki do usuniecia z systemu: ";
            std::cin >> nazwaDoniczki;

            if (baza.usunDoniczkeZSystemu(nazwaDoniczki)) {
                std::cout << "[SUKCES] Doniczka usunieta!" << std::endl;
            }
            else {
                std::cout << "[BLAD] Nie znaleziono takiej doniczki!" << std::endl;
            }
        }
        else if (komenda == "status_doniczki" || komenda == "4") {
            std::string nazwaDoniczki;
            std::cout << "Podaj nazwe doniczki do sprawdzenia: ";
            std::cin >> nazwaDoniczki;

            TDoniczka* d = TDoniczka::znajdzDoniczke(nazwaDoniczki);
            if (d != nullptr) {
                std::cout << "\nTATUS DONICZKI: " << d->pobierzNazweDoniczki() << " ===" << std::endl;
                std::cout << "Gatunek:             " << (d->pobierzGatunek() ? d->pobierzGatunek()->pobierzNazwe() : "Brak") << std::endl;
                std::cout << "Ostatnie podlanie:   " << d->pobierzOstatniePodlanie() << std::endl;

                std::cout << std::fixed << std::setprecision(1);
                std::cout << "Aktualna wilgotnosc: " << d->pobierzWilgotnosc() << " %" << std::endl;
                std::cout << std::defaultfloat;

                std::string przypisanyPokoj = baza.znajdzPokojDlaDoniczki(nazwaDoniczki);
                if (przypisanyPokoj == "Brak") {
                    std::cout << "Lokalizacja:         Doniczka nie jest obecnie w zadnym pokoju." << std::endl;
                }
                else {
                    std::cout << "Lokalizacja:         Przypisano do pokoju: " << przypisanyPokoj << std::endl;
                }
                std::cout << "====================================\n" << std::endl;
            }
            else {
                std::cout << "[BLAD] Nie ma takiej doniczki w systemie!" << std::endl;
            }
        }
        else if (komenda == "dodaj_pokoj" || komenda == "5") {
            std::string nazwa;
            std::cout << "Podaj nazwe pokoju: ";
            std::cin >> nazwa;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> rozkladTemp(15.0, 30.0);

            double losowaTemp = std::round(rozkladTemp(gen) * 10.0) / 10.0;

            baza.dodajPomieszczenie(new TPomieszczenie(nazwa, losowaTemp));

            std::cout << "[SUKCES] Pokoj dodany! Poczatkowa temperatura z czujnika to: "
                << losowaTemp << " st. C" << std::endl;
        }
        else if (komenda == "usun_pokoj" || komenda == "6") {
            std::string nazwaPokoju;
            std::cout << "Podaj nazwe pokoju do usuniecia z systemu: ";
            std::cin >> nazwaPokoju;
    // 2. Create the Qt Wrapper around the backend object
    DoniczkaWrapper uiDoniczka(&backendDoniczka);

            if (baza.usunPomieszczenieZSystemu(nazwaPokoju)) {
                std::cout << "[SUKCES] Pokoj usuniety!" << std::endl;
            }
            else {
                std::cout << "[BLAD] Nie znaleziono takiego pokoju!" << std::endl;
            }
        }
        else if (komenda == "przypisz_doniczke" || komenda == "7") {
            std::string nazwaPokoju, nazwaDoniczki;
            std::cout << "Do jakiego pokoju wstawic doniczke?: ";
            std::cin >> nazwaPokoju;
            std::cout << "Jaka doniczke tam wstawic?: ";
            std::cin >> nazwaDoniczki;

            TPomieszczenie* p = baza.pobierzPomieszczenie(nazwaPokoju);
            if (p != nullptr) {
                if (baza.czyDoniczkaPrzypisana(nazwaDoniczki)) {
                    std::cout << "[BLAD] Doniczka o tej nazwie znajduje sie juz w jakims pokoju!" << std::endl;
                }
                else {
                    if (p->dodajDoniczkePoNazwie(nazwaDoniczki)) {
                        baza.zapiszDoPliku();
                        std::cout << "[SUKCES] Doniczka przypisana do pokoju!" << std::endl;
                    }
                    else {
                        std::cout << "[BLAD] Taka doniczka nie istnieje w systemie!" << std::endl;
                    }
                }
            }
            else {
                std::cout << "[BLAD] Nie ma takiego pokoju w systemie!" << std::endl;
            }
        }
        else if (komenda == "usun_z_pokoju" || komenda == "8") {
            std::string nazwaPokoju, nazwaDoniczki;
            std::cout << "Z jakiego pokoju usunac?: ";
            std::cin >> nazwaPokoju;
            std::cout << "Jaka doniczke usunac?: ";
            std::cin >> nazwaDoniczki;

            TPomieszczenie* p = baza.pobierzPomieszczenie(nazwaPokoju);
            if (p != nullptr) {
                if (p->usunDoniczkePoNazwie(nazwaDoniczki)) {
                    baza.zapiszDoPliku();
                    std::cout << "[SUKCES] Usunięto doniczkę z pokoju!" << std::endl;
                }
                else {
                    std::cout << "[BLAD] W tym pokoju nie stoi taka doniczka!" << std::endl;
                }
            }
            else {
                std::cout << "[BLAD] Nie ma takiego pokoju w systemie!" << std::endl;
            }
        }
        else if (komenda == "info_pokoj" || komenda == "9") {
            std::string nazwaPokoju;
            std::cout << "Dla jakiego pokoju wygenerowac raport?: ";
            std::cin >> nazwaPokoju;
    QQmlApplicationEngine engine;

            TPomieszczenie* p = baza.pobierzPomieszczenie(nazwaPokoju);
            if (p != nullptr) {
                p->wyswietlZawartosc();
            }
            else {
                std::cout << "[BLAD] Nie ma takiego pokoju w systemie!" << std::endl;
            }
        }
        else if (komenda == "podlej" || komenda == "10") {
            std::string nazwaDoniczki;
            std::cout << "Podaj nazwe doniczki do podlania: ";
            std::cin >> nazwaDoniczki;
    // 3. Expose the WRAPPER to QML, not the backend directly
    engine.rootContext()->setContextProperty("DoniczkaController", &uiDoniczka);

            TDoniczka* d = TDoniczka::znajdzDoniczke(nazwaDoniczki);
            if (d != nullptr) {
                if (d->Podlewanie()) {
                    baza.zapiszDoPliku();
                    std::cout << "[SUKCES] Roslina podlana!" << std::endl;
                }
                else {
                    std::cout << std::fixed << std::setprecision(1);
                    std::cout << "[INFO] Wilgotnosc jest na poziomie " << d->pobierzWilgotnosc()
                        << "%. Gleba jest mokra - nie trzeba podlewac." << std::endl;
                    std::cout << std::defaultfloat;
                }
            }
            else {
                std::cout << "[BLAD] Nie ma takiej doniczki w systemie!" << std::endl;
            }
        }
        else if (komenda == "reguluj_termostat" || komenda == "11") {
            std::string nazwaPokoju;
            std::cout << "W jakim pokoju zoptymalizowac termostat?: ";
            std::cin >> nazwaPokoju;

            TPomieszczenie* p = baza.pobierzPomieszczenie(nazwaPokoju);
            if (p != nullptr) {
                p->regulujTermostat(baza.pobierzPokoje());
                baza.zapiszDoPliku();
            }
            else {
                std::cout << "[BLAD] Nie ma takiego pokoju w systemie!" << std::endl;
            }
        }
        else if (komenda == "wyjscie" || komenda == "12") {
            std::cout << "Zamykanie systemu. Do zobaczenia!" << std::endl;
            break;
        }
        else {
            std::cout << "[BLAD] Nieznana komenda. Sprobuj ponownie." << std::endl;
        }
    }
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/frontend/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}