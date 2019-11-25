INCLUDEPATH += . $(PWD)
TEMPLATE = app
CONFIG += release static NO_GUI
TARGET = appimageupdater
QT += core network

# Build time variables which will be used inside the application.
DEFINES += "APPIMAGE_UPDATER_COMMIT=\"\\\"$$(APPIMAGE_UPDATER_COMMIT)\\\"\""
DEFINES += "APPIMAGE_UPDATER_BUILD_NO=\"\\\"$$(APPIMAGE_UPDATER_BUILD_NO)\\\"\""
DEFINES += "APPIMAGE_UPDATER_BUILD_TIME=\"\\\"$$(APPIMAGE_UPDATER_BUILD_TIME)\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cc cutelog.c 
HEADERS += cutelog.h

include(../AppImageUpdaterBridge/AppImageUpdaterBridge.pri)
