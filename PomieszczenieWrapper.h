#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include "TPomieszczenie.h"
#include "DoniczkaWrapper.h"

class PomieszczenieWrapper : public QObject {
    Q_OBJECT
        Q_PROPERTY(QString nazwa READ getNazwa NOTIFY nazwaChanged)
        Q_PROPERTY(double temperatura READ getTemperatura WRITE setTemperatura NOTIFY temperaturaChanged)
        Q_PROPERTY(QList<QObject*> doniczki READ getDoniczki NOTIFY doniczkiChanged)

private:
    TPomieszczenie* m_pokoj;
    QList<QObject*> m_wrappedDoniczki; 

public:


    explicit PomieszczenieWrapper(TPomieszczenie* pokojBackend, QObject* parent = nullptr)
        : QObject(parent), m_pokoj(pokojBackend) {
        odswiezDoniczki();
    }

    ~PomieszczenieWrapper() {
        qDeleteAll(m_wrappedDoniczki);
    }

    QString getNazwa() const {
        return m_pokoj ? QString::fromUtf8(m_pokoj->getName().c_str()) : "";
    }

    double getTemperatura() const {
        return m_pokoj ? m_pokoj->getTemperature() : 0.0;
    }

    void setTemperatura(double nowaTemp) {
  
    }

    QList<QObject*> getDoniczki() const {
        return m_wrappedDoniczki;
    }

    void odswiezDoniczki() {
        qDeleteAll(m_wrappedDoniczki);
        m_wrappedDoniczki.clear();

        if (m_pokoj) {
            for (TDoniczka* d : m_pokoj->getPots()) {
                if (d) {
                    m_wrappedDoniczki.append(new DoniczkaWrapper(d, this));
                }
            }
        }
        emit doniczkiChanged();
    }

    void uruchomLogikeOstrzezen(const std::vector<TPomieszczenie*>& wszystkiePokoje) {
        if (!m_pokoj) return;
        double aktualnaTemp = m_pokoj->getTemperature();
        emit temperaturaChanged(); // Zmusza QML do zaktualizowania nagłówka z °C

        for (QObject* obj : m_wrappedDoniczki) {
            DoniczkaWrapper* dWrap = qobject_cast<DoniczkaWrapper*>(obj);
            if (!dWrap) continue;

            TDoniczka* bPot = dWrap->getBackendDoniczka();
            if (!bPot || !bPot->getKind()) continue;

            double tempDocelowa = bPot->getKind()->getTemperature();
            double uchyb = aktualnaTemp - tempDocelowa;

            if (std::abs(uchyb) > 5.0) {
                // uchyb < 0 -> w pokoju jest chłodniej niż roślina potrzebuje (śnieżka)
                // uchyb > 0 -> w pokoju jest cieplej niż roślina potrzebuje (ogień)
                dWrap->setStanTemperatury(uchyb < 0 ? 1 : 2);

                QString polecany = "";
                double minUchyb = 5.0; // Tolerancja max 5 stopni
                for (auto p : wszystkiePokoje) {
                    if (p && p->getName() != m_pokoj->getName()) {
                        double uInnym = std::abs(p->getTemperature() - tempDocelowa);
                        if (uInnym <= 5.0 && uInnym < minUchyb) {
                            minUchyb = uInnym;
                            polecany = QString::fromUtf8(p->getName().c_str());
                        }
                    }
                }
                dWrap->setSugerowanyPokoj(polecany);
            }
            else {
                dWrap->setStanTemperatury(0);
                dWrap->setSugerowanyPokoj("");
            }
        }
    }

signals:
    void nazwaChanged();
    void temperaturaChanged();
    void doniczkiChanged();
};