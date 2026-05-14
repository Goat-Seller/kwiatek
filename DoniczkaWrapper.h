#pragma once
#include <QObject>
#include <QString>
#include "TDoniczka.h" // Include your pure C++ backend header

class DoniczkaWrapper : public QObject {
    Q_OBJECT

        // QML Properties mapping to the C++ getters
        Q_PROPERTY(QString nazwaDoniczki READ getNazwa NOTIFY nazwaChanged)
        Q_PROPERTY(double aktualnaWilgotnosc READ getWilgotnosc NOTIFY wilgotnoscChanged)
        Q_PROPERTY(double aktualnaTemperatura READ getTemperatura NOTIFY temperaturaChanged)

public:
    // Pass a pointer to the actual backend object into the wrapper
    explicit DoniczkaWrapper(TDoniczka* doniczkaBackend, QObject* parent = nullptr)
        : QObject(parent), m_doniczka(doniczkaBackend) {
    }

    // Getters that read from the backend and convert to Qt types
    QString getNazwa() const {
         return QString::fromStdString(m_doniczka->getNazwa()); 
    }

    double getWilgotnosc() const {
         return m_doniczka->getWilgotnosc(); 
    }

    double getTemperatura() const {
         return m_doniczka->getTemperatura();
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
            m_doniczka->aktualizujWilgotnosc();
            m_doniczka->aktualizujTemperatura();
            emit wilgotnoscChanged();
            emit temperaturaChanged();
        }
    }

signals:
    void nazwaChanged();
    void wilgotnoscChanged();
    void temperaturaChanged();

private:
    TDoniczka* m_doniczka;
};