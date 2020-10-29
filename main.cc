#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QCommandLineParser>

#include <BuildConstants.hpp>
#include <SettingsManager.hpp>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("antony-jr");
    QApplication::setApplicationName("AppImageUpdater");

    qmlRegisterType<BuildConstants>("Core.BuildConstants", 1, 0, "BuildConstants");
    qmlRegisterType<SettingsManager>("Core.SettingsManager", 1, 0, "SettingsManager");
    
    app.setWindowIcon(QIcon(QString::fromUtf8(":/logo.png")));
    QQuickStyle::setStyle("Material"); // Use Google Material Design
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
