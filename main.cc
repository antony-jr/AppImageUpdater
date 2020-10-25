#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QCommandLineParser>

#ifndef APPIMAGE_UPDATER_VERSION
#define APPIMAGE_UPDATER_VERSION "2"
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("ShareMyHost");
    QApplication::setApplicationName("ShareMyHost");

    //qmlRegisterType<Clipper>("Core.Clipper", 1, 0, "Clipper");
    
    app.setWindowIcon(QIcon(QString::fromUtf8(":/logo.png")));
    QQuickStyle::setStyle("Material"); // Use Google Material Design
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
