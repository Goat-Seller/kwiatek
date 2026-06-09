#pragma once
#include <QObject>
#include <QString>
#include "TDoniczka.h" // Include your pure C++ backend header

class DoniczkaWrapper : public QObject {
    Q_OBJECT
        // QML Properties mapping to the C++ getters
      

public:
    Q_PROPERTY(QString nazwaDoniczki READ getPotName NOTIFY nazwaChanged)
    Q_PROPERTY(double aktualnaWilgotnosc READ getHumidity NOTIFY wilgotnoscChanged)
    Q_PROPERTY(QString nazwaGatunku READ getNazwaGatunku NOTIFY gatunekChanged)
    Q_PROPERTY(int stanTemperatury READ getStanTemperatury NOTIFY stanTemperaturyChanged)
    Q_PROPERTY(QString sugerowanyPokoj READ getSugerowanyPokoj NOTIFY sugerowanyPokojChanged)

    int getStanTemperatury() const { return m_stanTemperatury; }
    void setStanTemperatury(int stan) {
        if (m_stanTemperatury != stan) {
            m_stanTemperatury = stan;
            emit stanTemperaturyChanged();
        }
    }

    QString getSugerowanyPokoj() const { return m_sugerowanyPokoj; }
    void setSugerowanyPokoj(const QString& pokoj) {
        if (m_sugerowanyPokoj != pokoj) {
            m_sugerowanyPokoj = pokoj;
            emit sugerowanyPokojChanged();
        }
    }

    // Pass a pointer to the actual backend object into the wrapper
    explicit DoniczkaWrapper(TDoniczka* doniczkaBackend, QObject* parent = nullptr)
        : QObject(parent), m_doniczka(doniczkaBackend) {
    }

    // Getters that read from the backend and convert to Qt types
    QString getPotName() const {
        return QString::fromUtf8(m_doniczka->getPotName().c_str());
    }

    double getHumidity() const {
         return m_doniczka->getHumidity(); 
    }

    QString getNazwaGatunku() const {
        if (m_doniczka && m_doniczka->getKind()) {
            return QString::fromUtf8(m_doniczka->getKind()->getName().c_str());
        }
        return "Nieznany";
    }

    // Methods exposing to QML
    Q_INVOKABLE void podlej() {
        if (m_doniczka) {
            m_doniczka->Podlewanie(); // Call the pure C++ method
            emit wilgotnoscChanged(); // Tell QML the value updated
        }
    }

    Q_INVOKABLE void aktualizujCzujniki() {
        if (m_doniczka) {
            m_doniczka->setRandomHumidity();
            emit wilgotnoscChanged();
            emit temperaturaChanged();
        }
    }

    // Funkcja pomocnicza do symulacji upływu czasu
    void zmniejszWilgotnoscO(double procent) {
        if (m_doniczka) {
            double aktualna = m_doniczka->getHumidity();
            double nowa = aktualna - procent;

            // Zabezpieczenie przed ujemną wilgotnością
            if (nowa < 0.0) nowa = 0.0;

            m_doniczka->setHumidity(nowa);
            emit wilgotnoscChanged(); // Wymusza odświeżenie % w QML
        }
    }

    TDoniczka* getBackendDoniczka() const { return m_doniczka; }

signals:
    void nazwaChanged();
    void wilgotnoscChanged();
    void temperaturaChanged();
	void gatunekChanged();
    void stanTemperaturyChanged();
    void sugerowanyPokojChanged();

private:
    TDoniczka* m_doniczka;
    int m_stanTemperatury = 0; // 0 = ok, 1 = za zimno, 2 = za ciepło 
    QString m_sugerowanyPokoj = "";
};