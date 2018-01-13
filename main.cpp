#include <QApplication>
#include <QDesktopWidget>
#include "AppImageUpdater.hpp"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("AppImageUpdater");
    QApplication::setApplicationVersion(APPIMAGEUPDATER_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription("AppImageUpdater Lets you to Update your AppImages Like a Pro.");
    parser.addPositionalArgument("AppImage", "Path to the AppImage you wish to Update.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    AppImageUpdater MainApp;

    /*
     * Exit call
    */
    QObject::connect(&MainApp, &AppImageUpdater::safeToCloseApplication, &app, &QApplication::quit);

    /*
     * Set the Window to Center
    */
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-MainApp.width()) / 2;
    int y = (screenGeometry.height()-MainApp.height()) / 2;
    MainApp.move(x, y);
    MainApp.show();
    if(parser.positionalArguments().size() == 0) {
        MainApp.checkForUpdates(QString(argv[0]));
    } else {
        MainApp.checkForUpdates(parser.positionalArguments().at(0));
    }
    return app.exec();
}
