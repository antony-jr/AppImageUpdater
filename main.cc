#include <QApplication>
#include <QCommandLineParser>
#include <AppImageUpdater.hpp>
#include <AppImageUpdaterStandalone.hpp>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
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


    QCommandLineOption standalone(QStringList() << "standalone-update-dialog",
                                  QString::fromUtf8("Update a <AppImage> with nothing but a AppImageUpdater update dialog."),
                                  QString::fromUtf8("AppImage"));
    parser.addOption(standalone);
    parser.process(app);

    if(!parser.value(standalone).isEmpty()){ 
	    AppImageUpdaterStandalone standaloneDialogHandle(parser.value(standalone));
	    QObject::connect(&standaloneDialogHandle, &AppImageUpdaterStandalone::quit,
                     &app, &QApplication::quit, Qt::QueuedConnection);
    return app.exec();
    }

    AppImageUpdater mainWidget(parser.isSet(minimized));
    QObject::connect(&mainWidget, &AppImageUpdater::quit, &app, &QApplication::quit, Qt::QueuedConnection);
    return app.exec();
}
