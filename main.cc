#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QCommandLineParser>

#include <BuildConstants.hpp>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("ShareMyHost");
    QApplication::setApplicationName("ShareMyHost");

    qmlRegisterType<BuildConstants>("Core.BuildConstants", 1, 0, "BuildConstants");
    
    app.setWindowIcon(QIcon(QString::fromUtf8(":/logo.png")));
    QQuickStyle::setStyle("Material"); // Use Google Material Design
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
