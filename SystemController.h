#pragma once
#include <QObject>
#include <QStringList>
#include <QList>
#include "BazaRoslin.h"
#include "PomieszczenieWrapper.h"

class SystemController : public QObject {
    Q_OBJECT
        Q_PROPERTY(QList<QObject*> pomieszczenia READ getPomieszczenia NOTIFY pomieszczeniaChanged)
        Q_PROPERTY(QStringList gatunki READ getGatunki NOTIFY gatunkiChanged)

private:
    BazaRoslin* m_baza;
    QList<QObject*> m_wrappedPomieszczenia;

public:
    explicit SystemController(BazaRoslin* bazaBackend, QObject* parent = nullptr)
        : QObject(parent), m_baza(bazaBackend) {
        if (m_baza) {
            m_baza->wczytajZPliku();
        }
        generujWrapperyPomieszczen();
    }

    ~SystemController() {
        qDeleteAll(m_wrappedPomieszczenia);
    }

    QList<QObject*> getPomieszczenia() const {
        return m_wrappedPomieszczenia;
    }

        QStringList getGatunki() const {
            QStringList lista;
            if (m_baza) {
                for (TGatunek* g : m_baza->pobierzGatunki()) {
                    if (g) {
                        lista.append(QString::fromUtf8(g->getName().c_str()));
                    }
                }
            }

            return lista;
        }
        // Funkcja dodająca nowy pokój do systemu
       // Funkcja dodająca nowy pokój do systemu (zaktualizowana o temperaturę)
        Q_INVOKABLE QObject* dodajPomieszczenie(const QString& nazwaPokoju, double temperatura) {
            if (!m_baza || nazwaPokoju.trimmed().isEmpty()) return nullptr;

            // Zabezpieczenie przed dublowaniem nazw
            for (int i = 0; i < m_wrappedPomieszczenia.size(); ++i) {
                PomieszczenieWrapper* pWrap = qobject_cast<PomieszczenieWrapper*>(m_wrappedPomieszczenia[i]);
                if (pWrap && pWrap->getNazwa() == nazwaPokoju) {
                    return nullptr; // Taka nazwa już istnieje
                }
            }

            // Tworzymy nowe pomieszczenie z podaną temperaturą
            TPomieszczenie* nowyPokoj = new TPomieszczenie(nazwaPokoju.toUtf8().constData(), temperatura);
            m_baza->dodajPomieszczenie(nowyPokoj);

            PomieszczenieWrapper* nowyWrapper = new PomieszczenieWrapper(nowyPokoj, this);
            m_wrappedPomieszczenia.append(nowyWrapper);

            emit pomieszczeniaChanged();
            return nowyWrapper;
        }

        // Funkcja wywoływana z QML po kliknięciu "Usuń pokój"
        Q_INVOKABLE void usunPomieszczenie(const QString& nazwaPokoju) {
            if (!m_baza) return;

            // 1. Najpierw szukamy i usuwamy wrapper z listy UI, dopóki obiekt backendu istnieje
            for (int i = 0; i < m_wrappedPomieszczenia.size(); ++i) {
                PomieszczenieWrapper* pWrap = qobject_cast<PomieszczenieWrapper*>(m_wrappedPomieszczenia[i]);
                if (pWrap && pWrap->getNazwa() == nazwaPokoju) {
                    m_wrappedPomieszczenia.removeAt(i);
                    pWrap->deleteLater(); // Bezpieczne usunięcie obiektu QObject przez Qt
                    break;
                }
            }

            // 2. Dopiero teraz bezpiecznie usuwamy obiekt z pamięci backendu i zapisujemy plik JSON
            m_baza->usunPomieszczenieZSystemu(nazwaPokoju.toUtf8().constData());

            // 3. Informujemy interfejs QML, że struktura siatki uległa zmianie
            emit pomieszczeniaChanged();
        }

    // Funkcja wywoływana automatycznie po kliknięciu "Zapisz" w QML
    Q_INVOKABLE void dodajDoniczkeDoPokoju(const QString& nazwaPokoju, const QString& nazwaDoniczki, const QString& nazwaGatunku) {
        if (!m_baza) return;

        // 1. Szukamy wzorca gatunku w bazie danych
        TGatunek* znalezionyGatunek = nullptr;
        for (TGatunek* g : m_baza->pobierzGatunki()) {
            if (g && g->getName() == nazwaGatunku.toUtf8().constData()) {
                znalezionyGatunek = g;
                break;
            }
        }
        if (!znalezionyGatunek) return;

        // 2. Tworzymy nowy backendowy obiekt doniczki i rejestrujemy w bazie
        TDoniczka* nowaDoniczka = new TDoniczka(nazwaDoniczki.toUtf8().constData(), znalezionyGatunek);
        m_baza->dodajDoniczke(nowaDoniczka);

        // 3. Przypisujemy doniczkę do odpowiedniego pokoju w C++
        TPomieszczenie* pokoj = m_baza->pobierzPomieszczenie(nazwaPokoju.toUtf8().constData());
        if (pokoj) {
            pokoj->addPotByName(nowaDoniczka->getPotName());
        }

        // Zapisujemy stan do pliku JSON
        m_baza->zapiszDoPliku();

        // 4. Odświeżamy widok konkretnego wrappera, by natychmiast pokazał nowy kafelek w QML
        for (QObject* obj : m_wrappedPomieszczenia) {
            PomieszczenieWrapper* pWrap = qobject_cast<PomieszczenieWrapper*>(obj);
            if (pWrap && pWrap->getNazwa() == nazwaPokoju) {
                pWrap->odswiezDoniczki();
                break;
            }
        }
    }

    Q_INVOKABLE void uruchomTermostat(const QString& nazwaPokoju) {
        if (!m_baza) return;

        TPomieszczenie* pokoj = m_baza->pobierzPomieszczenie(nazwaPokoju.toUtf8().constData());
        if (!pokoj) return;

        // 1. Zlecenie przeliczenia średniej do czystego backendu C++
        pokoj->regulateTemperature(m_baza->pobierzPokoje());
        m_baza->zapiszDoPliku(); // Zapisujemy nowe wyniki do JSON

        // 2. Znalezienie odpowiedniego wrappera UI i odpalenie logiki widoku
        for (int i = 0; i < m_wrappedPomieszczenia.size(); ++i) {
            PomieszczenieWrapper* pWrap = qobject_cast<PomieszczenieWrapper*>(m_wrappedPomieszczenia[i]);
            if (pWrap && pWrap->getNazwa() == nazwaPokoju) {
                pWrap->uruchomLogikeOstrzezen(m_baza->pobierzPokoje());
                break;
            }
        }
    }

    // Wywoływane z QML po kliknięciu przycisku "Symuluj czas"
    Q_INVOKABLE void symulujUplywCzasu() {
        if (!m_baza) return;

        // Przechodzimy przez wszystkie aktywne pokoje w systemie
        for (int i = 0; i < m_wrappedPomieszczenia.size(); ++i) {
            PomieszczenieWrapper* pWrap = qobject_cast<PomieszczenieWrapper*>(m_wrappedPomieszczenia[i]);
            if (pWrap) {
                // Wyciągamy doniczki w danym pokoju
                QList<QObject*> doniczki = pWrap->getDoniczki();
                for (int j = 0; j < doniczki.size(); ++j) {
                    DoniczkaWrapper* dWrap = qobject_cast<DoniczkaWrapper*>(doniczki[j]);
                    if (dWrap) {
                        dWrap->zmniejszWilgotnoscO(15.0); 
                    }
                }
            }
        }

        m_baza->zapiszDoPliku();
    }


    // Funkcja usuwająca doniczkę (z pokoju i z bazy)
    Q_INVOKABLE void usunDoniczke(const QString& nazwaPokoju, const QString& nazwaDoniczki) {
        if (!m_baza) return;

        bool usunieto = m_baza->usunDoniczkeZSystemu(nazwaDoniczki.toUtf8().constData());

        if (usunieto) {
            for (int i = 0; i < m_wrappedPomieszczenia.size(); ++i) {
                PomieszczenieWrapper* pWrap = qobject_cast<PomieszczenieWrapper*>(m_wrappedPomieszczenia[i]);
                if (pWrap && pWrap->getNazwa() == nazwaPokoju) {
                    pWrap->odswiezDoniczki();
                    break;
                }
            }
        }
    }

    // Funkcja przenosząca doniczkę między pokojami
    Q_INVOKABLE void przeniesDoniczke(const QString& nazwaDoniczki, const QString& staryPokoj, const QString& nowyPokoj) {
        if (!m_baza || staryPokoj == nowyPokoj) return;

        TPomieszczenie* pStary = m_baza->pobierzPomieszczenie(staryPokoj.toUtf8().constData());
        TPomieszczenie* pNowy = m_baza->pobierzPomieszczenie(nowyPokoj.toUtf8().constData());

        if (pStary && pNowy) {
            pStary->deletePotByName(nazwaDoniczki.toUtf8().constData());
            pNowy->addPotByName(nazwaDoniczki.toUtf8().constData());

            m_baza->zapiszDoPliku();

            for (int i = 0; i < m_wrappedPomieszczenia.size(); ++i) {
                PomieszczenieWrapper* pWrap = qobject_cast<PomieszczenieWrapper*>(m_wrappedPomieszczenia[i]);
                if (pWrap) {
                    if (pWrap->getNazwa() == staryPokoj || pWrap->getNazwa() == nowyPokoj) {
                        pWrap->odswiezDoniczki();
                    }
                }
            }
        }
    }

    // Pobieranie minimalnej wilgotności dla danego gatunku
    Q_INVOKABLE double getGatunekMinHumidity(const QString& nazwaGatunku) {
        if (!m_baza) return 0.0;
        for (TGatunek* g : m_baza->pobierzGatunki()) {
            if (g && QString::fromUtf8(g->getName().c_str()) == nazwaGatunku) {
                return g->getMinHumidity();
            }
        }
        return 0.0;
    }

    // Pobieranie docelowej temperatury dla danego gatunku
    Q_INVOKABLE double getGatunekTemperature(const QString& nazwaGatunku) {
        if (!m_baza) return 0.0;
        for (TGatunek* g : m_baza->pobierzGatunki()) {
            if (g && QString::fromUtf8(g->getName().c_str()) == nazwaGatunku) {
                return g->getTemperature();
            }
        }
        return 0.0;
    }

    // Tworzenie zupełnie nowego gatunku rośliny
    Q_INVOKABLE bool dodajGatunek(const QString& nazwa, double minWilg, double temp) {
        if (!m_baza || nazwa.trimmed().isEmpty()) return false;

        // Blokada duplikatów
        for (TGatunek* g : m_baza->pobierzGatunki()) {
            if (g && QString::fromUtf8(g->getName().c_str()) == nazwa) return false;
        }

        TGatunek* nowyGatunek = new TGatunek(nazwa.toUtf8().constData(), minWilg, temp);
        m_baza->dodajGatunek(nowyGatunek);
        m_baza->zapiszDoPliku();

        emit gatunkiChanged(); // Odświeża listy w całej aplikacji
        return true;
    }

    // Zmiana parametrów istniejącego gatunku
    Q_INVOKABLE void aktualizujGatunek(const QString& nazwa, double minWilg, double temp) {
        if (!m_baza) return;
        for (TGatunek* g : m_baza->pobierzGatunki()) {
            if (g && QString::fromUtf8(g->getName().c_str()) == nazwa) {
                g->setMinHumidity(minWilg);
                g->setTemperature(temp);
                break;
            }
        }
        m_baza->zapiszDoPliku();
        emit gatunkiChanged();
    }

    // Usunięcie gatunku z bazy danych
    Q_INVOKABLE void usunGatunek(const QString& nazwa) {
        if (!m_baza) return;

        bool usunieto = m_baza->usunGatunekZSystemu(nazwa.toUtf8().constData());

        if (usunieto) {
            // Ponieważ usunięcie gatunku usuwa też doniczki, wymuszamy odświeżenie wszystkich pokoi
            for (int i = 0; i < m_wrappedPomieszczenia.size(); ++i) {
                PomieszczenieWrapper* pWrap = qobject_cast<PomieszczenieWrapper*>(m_wrappedPomieszczenia[i]);
                if (pWrap) {
                    pWrap->odswiezDoniczki();
                }
            }

            // Odświeżamy listy gatunków w QML
            emit gatunkiChanged();
        }
    }

    void generujWrapperyPomieszczen() {
        qDeleteAll(m_wrappedPomieszczenia);
        m_wrappedPomieszczenia.clear();

        if (m_baza) {
            // Pobieramy wektor pokoi (pobierzPokoje() z BazaRoslin)
            for (TPomieszczenie* p : m_baza->pobierzPokoje()) {
                if (p) {
                    m_wrappedPomieszczenia.append(new PomieszczenieWrapper(p, this));
                }
            }
        }
        emit pomieszczeniaChanged();
    }

signals:
    void pomieszczeniaChanged();
    void gatunkiChanged();
};