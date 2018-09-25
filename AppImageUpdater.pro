include(AppImageUpdaterBridge/AppImageUpdaterBridge.pri)
include(SingleApplication/singleapplication.pri)
INCLUDEPATH += . $(PWD)
TEMPLATE = app
LIBS += -lutil
CONFIG += release static
TARGET = AppImageUpdater
QT += core network gui widgets concurrent

# Build time variables which will be used inside the application.
DEFINES += "APPIMAGE_UPDATER_VERSION=\"\\\"$$(APPIMAGE_UPDATER_VERSION)\\\"\""
DEFINES += "APPIMAGE_UPDATER_COMMIT=\"\\\"$$(APPIMAGE_UPDATER_COMMIT)\\\"\""
DEFINES += "APPIMAGE_UPDATER_BUILD_NO=\"\\\"$$(APPIMAGE_UPDATER_BUILD_NO)\\\"\""
DEFINES += "APPIMAGE_UPDATER_BUILD_TIME=\"\\\"$$(APPIMAGE_UPDATER_BUILD_TIME)\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += AppImageUpdater.hpp AppImageUpdaterStandalone.hpp SettingsDialog.hpp AuthorizationDialog.hpp 
SOURCES += AppImageUpdater.cc AppImageUpdaterStandalone.cc SettingsDialog.cc AuthorizationDialog.cc main.cc
RESOURCES += app_resources/resources.qrc
FORMS += AppImageUpdater.ui SettingsDialog.ui AuthorizationDialog.ui
