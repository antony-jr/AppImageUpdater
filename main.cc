#include <QApplication>
#include <AppImageUpdater.hpp>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QTimer timer;
    QApplication::setApplicationName("AppImageUpdater");
    QApplication::setApplicationVersion(APPIMAGE_UPDATER_VERSION);
    
    AppImageUpdater mainWidget;
    
    /*  Exit call. */
    QObject::connect(&mainWidget, &AppImageUpdater::quit , &app, &QApplication::quit , Qt::QueuedConnection);
    return app.exec();
}
