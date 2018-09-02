include(AppImageUpdaterBridge/AppImageUpdaterBridge.pri)
INCLUDEPATH += . $(PWD)
TEMPLATE = app
CONFIG += release static
TARGET = AppImageUpdater
QT += core network gui widgets

# Build time variables which will be used inside the application.
DEFINES += "APPIMAGE_UPDATER_VERSION=\"\\\"$$(APPIMAGE_UPDATER_VERSION)\\\"\""
DEFINES += "APPIMAGE_UPDATER_COMMIT=\"\\\"$$(APPIMAGE_UPDATER_COMMIT)\\\"\""
DEFINES += "APPIMAGE_UPDATER_BUILD_NO=\"\\\"$$(APPIMAGE_UPDATER_BUILD_NO)\\\"\""
DEFINES += "APPIMAGE_UPDATER_BUILD_TIME=\"\\\"$$(APPIMAGE_UPDATER_BUILD_TIME)\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += AppImageUpdater.hpp
SOURCES += AppImageUpdater.cc main.cc
RESOURCES += app_resources/resources.qrc
FORMS += AppImageUpdater.ui
