#
# The BSD-3 Clause License.
# Copyright (c) 2018 Antony Jr.
# 
# QMake Project file to compile AppImageUpdaterBridge statically.
# ----
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = AppImageUpdaterBridge \
          src

INCLUDEPATH = .
