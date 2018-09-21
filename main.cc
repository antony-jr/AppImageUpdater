#include <SingleApplication>
#include <AppImageUpdater.hpp>

int main(int argc, char **argv)
{
    SingleApplication app(argc, argv); 
    app.setQuitOnLastWindowClosed(false);

    QApplication::setOrganizationName("AppImage");
    QApplication::setApplicationName("AppImageUpdater");
    QApplication::setApplicationVersion(APPIMAGE_UPDATER_VERSION);

    AppImageUpdater mainWidget;
    
    /* Wake if new instances are opened. */
    QObject::connect(&app , &SingleApplication::instanceStarted , &mainWidget , &AppImageUpdater::gracefulShow);

    /*  Exit call. */
    QObject::connect(&mainWidget, &AppImageUpdater::quit , &app, &SingleApplication::quit , Qt::QueuedConnection);
    return app.exec();
}
