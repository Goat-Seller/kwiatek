#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "BazaRoslin.h"
#include "SystemController.h"

int main(int argc, char* argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "Fusion");
    QGuiApplication app(argc, argv);

    // 1. Tworzymy główny backend logiczny (wskazując ścieżkę do bazy json)
    BazaRoslin backendBazy("C:/studia/semestr-6/POiO/kwaitek/baza_relacyjna.json");


    // 2. Inicjalizujemy nasz nadrzędny sterownik systemu Qt
    SystemController systemController(&backendBazy);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("SystemController", &systemController);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}