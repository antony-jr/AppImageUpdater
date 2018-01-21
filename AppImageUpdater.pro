# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr.
# All Rights Reserved.
# ==========================================
# Helps you to build this library statically 
# with Qt static version , Since CMake does
# not go well with Qt Static linking.
# ==========================================
# MUST COMPLY WITH LGPL IF STATICALLY LINKED
# ******************************************
TEMPLATE = app
CONFIG += release
TARGET = AppImageUpdater
LIBS += -lcurl
QT += core gui widgets network concurrent
INCLUDEPATH += . AppImageUpdaterBridge AppImageUpdaterBridge/QAIUpdateInformation AppImageUpdaterBridge/libzsync AppImageUpdaterBridge/librcksum AppImageUpdaterBridge/libz 

# Input
HEADERS += AppImageUpdater.hpp \
           AppImageUpdaterBridge/AIUpdaterBridge.hpp \
           AppImageUpdaterBridge/config.h \
           AppImageUpdaterBridge/format_string.h \
           AppImageUpdaterBridge/legacy_http.h \
           AppImageUpdaterBridge/zsglobal.h \
           AppImageUpdaterBridge/librcksum/internal.h \
           AppImageUpdaterBridge/librcksum/md4.h \
           AppImageUpdaterBridge/librcksum/rcksum.h \
           AppImageUpdaterBridge/libzsync/sha1.h \
           AppImageUpdaterBridge/libzsync/zmap.h \
           AppImageUpdaterBridge/libzsync/zsync.h \
           AppImageUpdaterBridge/QAIUpdateInformation/QAIUpdateInformation.hpp \
           AppImageUpdaterBridge/zlib/crc32.h \
           AppImageUpdaterBridge/zlib/deflate.h \
           AppImageUpdaterBridge/zlib/inffixed.h \
           AppImageUpdaterBridge/zlib/inflate.h \
           AppImageUpdaterBridge/zlib/inftrees.h \
           AppImageUpdaterBridge/zlib/trees.h \
           AppImageUpdaterBridge/zlib/zconf.h \
           AppImageUpdaterBridge/zlib/zlib.h \
           AppImageUpdaterBridge/zlib/zutil.h
SOURCES += AppImageUpdater.cpp \
           main.cpp \
           AppImageUpdaterBridge/AIUpdaterBridge.cpp \
           AppImageUpdaterBridge/legacy_http.c \
           AppImageUpdaterBridge/librcksum/hash.c \
           AppImageUpdaterBridge/librcksum/md4.c \
           AppImageUpdaterBridge/librcksum/range.c \
           AppImageUpdaterBridge/librcksum/rsum.c \
           AppImageUpdaterBridge/librcksum/state.c \
           AppImageUpdaterBridge/libzsync/sha1.c \
           AppImageUpdaterBridge/libzsync/zmap.c \
           AppImageUpdaterBridge/libzsync/zsync.c \
           AppImageUpdaterBridge/QAIUpdateInformation/QAIUpdateInformation.cpp \
           AppImageUpdaterBridge/zlib/adler32.c \
           AppImageUpdaterBridge/zlib/compress.c \
           AppImageUpdaterBridge/zlib/crc32.c \
           AppImageUpdaterBridge/zlib/deflate.c \
           AppImageUpdaterBridge/zlib/gzio.c \
           AppImageUpdaterBridge/zlib/inflate.c \
           AppImageUpdaterBridge/zlib/inftrees.c \
           AppImageUpdaterBridge/zlib/trees.c \
           AppImageUpdaterBridge/zlib/zutil.c 
RESOURCES += AppImageUpdaterResources.qrc
