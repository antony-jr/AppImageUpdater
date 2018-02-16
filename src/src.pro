TEMPLATE = app
CONFIG += release static
TARGET = AppImageUpdater
LIBS += ../AppImageUpdaterBridge/src/libAIUpdaterBridge.a \
        ../AppImageUpdaterBridge/libzsync/libzsync.a  \
        ../AppImageUpdaterBridge/zlib/libz.a \
        ../AppImageUpdaterBridge/librcksum/librcksum.a \
        ../AppImageUpdaterBridge/QAIUpdateInformation/libQAIUpdateInformation.a -lcurl

QT += core network concurrent gui widgets
INCLUDEPATH += . .. ../AppImageUpdaterBridge \
	       ../AppImageUpdaterBridge/src \
	       ../AppImageUpdaterBridge/QAIUpdateInformation \
	       ../AppImageUpdaterBridge/libzsync \
	       ../AppImageUpdaterBridge/librcksum \
	       ../AppImageUpdaterBridge/libz

DEFINES += QT_DEPRECATED_WARNINGS

# Input
HEADERS += AppImageUpdater.hpp 
SOURCES += AppImageUpdater.cpp main.cpp
RESOURCES += ../AppImageUpdaterResources.qrc
