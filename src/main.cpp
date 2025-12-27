#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "particle_system.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Register C++ types for QML
    qmlRegisterType<ParticleSystem>("ParticleSystem", 1, 0, "ParticleSystem");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("particles", "Main");

    return app.exec();
}
