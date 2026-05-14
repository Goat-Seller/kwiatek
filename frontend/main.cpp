#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> 
#include "TDoniczka.h" 

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    TDoniczka mojaDoniczka("Fikus 1", "Fikus Benjamina");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("DoniczkaController", &mojaDoniczka);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/frontend/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
