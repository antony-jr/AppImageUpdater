#include <QApplication>
#include <QCommandLineParser>
#include <QAppImageUpdate>
#include <AppImageUpdater.hpp>
#include <AppImageUpdaterStandalone.hpp>

#ifndef APPIMAGE_UPDATER_VERSION
#define APPIMAGE_UPDATER_VERSION "2"
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    int standaloneFlags = QAppImageUpdate::GuiFlag::Default ^ QAppImageUpdate::GuiFlag::ShowBeforeProgress; 

    app.setQuitOnLastWindowClosed(false);
    QApplication::setOrganizationName("AppImage");
    QApplication::setApplicationName("AppImageUpdater");
    QApplication::setApplicationVersion(APPIMAGE_UPDATER_VERSION);


    QCommandLineParser parser;
    parser.setApplicationDescription(QString::fromUtf8("A Simple Program to Update the AppImage format using Zsync Algorithm."));
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
		    standaloneFlags |= QAppImageUpdate::GuiFlag::NoConfirmTorrentUsage;
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

    AppImageUpdater mainWidget(parser.isSet(minimized));
    QObject::connect(&mainWidget, &AppImageUpdater::quit, &app, &QApplication::quit, Qt::QueuedConnection);
    return app.exec();
}
