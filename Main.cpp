#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// Include your backend and wrapper
#include "TDoniczka.h"
#include "DoniczkaWrapper.h"

int main(int argc, char* argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qputenv("QT_QUICK_CONTROLS_STYLE", "Fusion");

    QGuiApplication app(argc, argv);


    // 1. Create pure C++ backend object
    TDoniczka backendDoniczka("Doniczka 1", "Fikus");

    // 2. Create the Qt Wrapper around the backend object
    DoniczkaWrapper uiDoniczka(&backendDoniczka);

    QQmlApplicationEngine engine;

    // 3. Expose the WRAPPER to QML, not the backend directly
    engine.rootContext()->setContextProperty("DoniczkaController", &uiDoniczka);

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/frontend/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}