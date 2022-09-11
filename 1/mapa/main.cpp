#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mapprovider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MapProvider>("game.engine", 1, 0, "MapProvider");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

