#include <iostream>
#include <QLockFile>
#include <QDir>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QCommandLineParser>
#include <QScopedPointer>

#include "AppImageUpdaterStandalone.hpp"

#include "BuildConstants.hpp"
#include "SettingsManager.hpp"
#include "SystemTray.hpp"
#include "DropItemParser.hpp"
#include "Updater.hpp"
#include "SeedManager.hpp"
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
    std::cout << "AppImage Updater v" << APPIMAGE_UPDATER_VERSION << "(" << APPIMAGE_UPDATER_COMMIT << "), "
	      << "AppImage Delta Updater for Humans.\n"
	      << "Copyright (C) Antony Jr.\n\n"; 

    QLockFile lockFile(QDir::homePath() + "/"  + ".AppImageUpdater.lock");
    QApplication app(argc, argv);
    QApplication::setOrganizationName("antony-jr");
    QApplication::setApplicationName("AppImage Updater");
    
    int standaloneFlags =  (QAppImageUpdate::GuiFlag::Default | QAppImageUpdate::GuiFlag::NoShowErrorDialogOnPermissionErrors)  
	    		   ^ QAppImageUpdate::GuiFlag::ShowBeforeProgress;
    QCommandLineParser parser;
    
    parser.setApplicationDescription(QString::fromUtf8("AppImage Updater for Humans."));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption minimized(QString::fromUtf8("minimized"), QString::fromUtf8("Start the Application minimized."));
    parser.addOption(minimized);
    
    QCommandLineOption noconfirm(QStringList() << "noconfirm" << "n" ,
		                   QString::fromUtf8("Automatically accept all confirmations in standalone mode."));
    parser.addOption(noconfirm);

    QCommandLineOption silent(QStringList() << "silent" << "s", QString::fromUtf8("Show minimum number of dialogs in standalone mode."));
    parser.addOption(silent);


    QCommandLineOption beforeProgress(QStringList() << "show-checking-for-update-dialog" << "c", QString::fromUtf8("Show Checking for update dialog in standalone mode."));
    parser.addOption(beforeProgress);



    QCommandLineOption standalone(QStringList() << "standalone-update-dialog" << "d",
                                  QString::fromUtf8("Update a <AppImage> with nothing but a AppImageUpdater update dialog."),
                                  QString::fromUtf8("AppImage"));
    parser.addOption(standalone);
    parser.process(app);

    if(!parser.value(standalone).isEmpty()){ 
	    if(parser.isSet(noconfirm)){
		    standaloneFlags ^= QAppImageUpdate::GuiFlag::ShowUpdateConfirmationDialog;
	    }

	    if(parser.isSet(beforeProgress)){
		    standaloneFlags |= QAppImageUpdate::GuiFlag::ShowBeforeProgress;
	    }

	    if(parser.isSet(silent)){
		    standaloneFlags ^= QAppImageUpdate::GuiFlag::ShowProgressDialog;
	    }

	    AppImageUpdaterStandalone standaloneDialogHandle(parser.value(standalone), standaloneFlags);
	    QObject::connect(&standaloneDialogHandle, &AppImageUpdaterStandalone::quit,
                     &app, &QApplication::quit, Qt::QueuedConnection);
	    standaloneDialogHandle.init();
	    return app.exec();
    }

    if(!lockFile.tryLock()) {
	    if(!lockFile.removeStaleLockFile()) {
		    return 0;
	    }

	    if(!lockFile.tryLock()) {
		    return 0;
	    }
    }
 
    app.setQuitOnLastWindowClosed(false);

    qmlRegisterType<BuildConstants>("Core.BuildConstants", 1, 0, "BuildConstants");
    qmlRegisterType<SettingsManager>("Core.SettingsManager", 1, 0, "SettingsManager");
    qmlRegisterType<SystemTray>("Core.SystemTray", 1, 0, "SystemTray");
    qmlRegisterType<DropItemParser>("Core.DropItemParser", 1, 0, "DropItemParser");
    qmlRegisterType<Updater>("Core.Updater", 1, 0, "Updater");
    qmlRegisterType<SeedManager>("Core.SeedManager", 1, 0, "SeedManager");
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
