#include <QApplication>
#include <QCommandLineParser>
#include <AppImageUpdater.hpp>
#include <AppImageUpdaterStandalone.hpp>
#include <AppImageUpdaterDialog>

using AppImageUpdaterBridge::AppImageUpdaterDialog;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    int standaloneFlags = AppImageUpdaterDialog::Default;

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

    QCommandLineOption standalone(QStringList() << "standalone-update-dialog" << "d",
                                  QString::fromUtf8("Update a <AppImage> with nothing but a AppImageUpdater update dialog."),
                                  QString::fromUtf8("AppImage"));
    parser.addOption(standalone);
    parser.process(app);

    if(!parser.value(standalone).isEmpty()){ 
	    if(parser.isSet(noconfirm)){
		    standaloneFlags ^= AppImageUpdaterDialog::ShowUpdateConfirmationDialog;
	    }

	    if(parser.isSet(silent)){
		    standaloneFlags ^= AppImageUpdaterDialog::ShowProgressDialog;
	    	    standaloneFlags ^= AppImageUpdaterDialog::ShowBeforeProgress; 
	
	    }else{
		    standaloneFlags |= AppImageUpdaterDialog::ShowBeforeProgress; 
	    }

	    AppImageUpdaterStandalone standaloneDialogHandle(parser.value(standalone), standaloneFlags);
	    QObject::connect(&standaloneDialogHandle, &AppImageUpdaterStandalone::quit,
                     &app, &QApplication::quit, Qt::QueuedConnection);
	    return app.exec();
    }

    AppImageUpdater mainWidget(parser.isSet(minimized));
    QObject::connect(&mainWidget, &AppImageUpdater::quit, &app, &QApplication::quit, Qt::QueuedConnection);
    return app.exec();
}
