#include <iostream>
#include <QApplication>
#include <SingleApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QCommandLineParser>

#include "BuildConstants.hpp"
#include "SettingsManager.hpp"
#include "SystemTray.hpp"
#include "DropItemParser.hpp"
#include "Updater.hpp"
#include "Executer.hpp"
#include "Helpers.hpp"

#include "global.hpp"
#include "AppImageImageProvider.hpp"

#ifndef APPIMAGE_UPDATER_VERSION
#define APPIMAGE_UPDATER_VERSION "2"
#endif
#ifndef APPIMAGE_UPDATER_COMMIT
#define APPIMAGE_UPDATER_COMMIT "none"
#endif


AppImageImageProvider *g_AppImageImageProvider = nullptr;

int main(int argc, char **argv)
{
    if(argc != 1 && !strcmp(argv[1], "--private")) {
	    QCoreApplication app(argc, argv);
	    return app.exec();
    }

    std::cout << "AppImage Updater v" << APPIMAGE_UPDATER_VERSION << "(" << APPIMAGE_UPDATER_COMMIT << "), "
	      << "AppImage Delta Updater for Humans.\n"
	      << "Copyright (C) Antony Jr.\n\n"; 

    SingleApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    SingleApplication::setOrganizationName("antony-jr");
    SingleApplication::setApplicationName("AppImage Updater");

    qmlRegisterType<BuildConstants>("Core.BuildConstants", 1, 0, "BuildConstants");
    qmlRegisterType<SettingsManager>("Core.SettingsManager", 1, 0, "SettingsManager");
    qmlRegisterType<SystemTray>("Core.SystemTray", 1, 0, "SystemTray");
    qmlRegisterType<DropItemParser>("Core.DropItemParser", 1, 0, "DropItemParser");
    qmlRegisterType<Updater>("Core.Updater", 1, 0, "Updater");
    qmlRegisterType<Executer>("Core.Executer", 1, 0, "Executer");
    qmlRegisterType<Helpers>("Core.Helpers", 1, 0, "Helpers");

    app.setWindowIcon(QIcon(QString::fromUtf8(":/logo.png")));
    QQuickStyle::setStyle("Material"); // Use Google Material Design
    QQmlApplicationEngine engine;
    
    g_AppImageImageProvider = new AppImageImageProvider;
    engine.addImageProvider("AIImage", g_AppImageImageProvider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
