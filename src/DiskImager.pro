TEMPLATE = app
TARGET = RasplexInstaller
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
DEFINES -= UNICODE
#DEFINES += QT_NO_CAST_FROM_ASCII
VERSION = 0.7
VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"
QMAKE_TARGET_PRODUCT = "Installer for RasPlex"
QMAKE_TARGET_DESCRIPTION = "Installer for RasPlex Plex client for Raspberry Pi, used to write SD cards from Sourceforge Downloads"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2009-2013 Rasplex"

# Input
HEADERS += disk.h mainwindow.h droppablelineedit.h
FORMS += mainwindow.ui
SOURCES += disk.cpp main.cpp mainwindow.cpp droppablelineedit.cpp
RESOURCES += gui_icons.qrc
RC_FILE = DiskImager.rc
TRANSLATIONS  = diskimager_en.ts

