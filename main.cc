#include <QApplication>
#include <QDesktopWidget>
#include <AppImageUpdater.hpp>


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("AppImageUpdater");
    QApplication::setApplicationVersion(APPIMAGE_UPDATER_VERSION);
    
    AppImageUpdater appWidget;
    
    /*  Exit call. */
    QObject::connect(&appWidget, &AppImageUpdater::quit , &app, &QApplication::quit , Qt::QueuedConnection);
    /* Set the window to center. */
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-appWidget.width()) / 2;
    int y = (screenGeometry.height()-appWidget.height()) / 2;
    appWidget.move(x, y);
    appWidget.show();
    return app.exec();
}
