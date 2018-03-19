#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T13:52:09
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets network quick qml

win32:QT += winextras

TARGET = out/gm-companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += gm-companion/main.cpp \
    # UI
#    gm-companion/ui/whatisnewwindow.cpp \
#    gm-companion/ui/updatedialog.cpp \
#    gm-companion/ui/aboutdialog.cpp \
#    # Other
    gm-companion/functions.cpp \
    # Tools
    gm-companion/tools/audio/audiotool.cpp \
    gm-companion/tools/audio/audioeditor.cpp \
    gm-companion/tools/maptool.cpp \
    gm-companion/tools/dicetool.cpp \
    gm-companion/tools/combattracker.cpp \
    gm-companion/tools/notestool.cpp \
    gm-companion/tools/convertertool.cpp \
    gm-companion/tools/generators/namegenerator.cpp \
    # Tools ADDONS
#    gm-companion/tools/addons/sifrp.cpp \
    # Tools CHARACTERS
#    gm-companion/tools/characters/sheets/defaultsheet.cpp \
#    gm-companion/tools/characters/sheets/sheetfunctions.cpp \
#    gm-companion/tools/characters/sheets/dsa5sheet.cpp \
#    gm-companion/tools/characters/charactertool.cpp \
#    gm-companion/tools/characters/sheets/entaria2sheet.cpp \
#    gm-companion/tools/characters/sheets/dnd5esheet.cpp
    # Tools ITEMS
#    gm-companion/tools/items/shoptool.cpp \
#    gm-companion/tools/items/shopeditor.cpp \
#    gm-companion/tools/items/itemeditor.cpp \
    # Managers
#    gm-companion/managers/filemanager.cpp \
    gm-companion/managers/updatemanager.cpp \
    # Settings
    gm-companion/settings/settingstool.cpp \
    gm-companion/settings/settingsmanager.cpp \
#    gm-companion/tools/addons/addonmanager.cpp \
    gm-companion/platformdetails.cpp

lupdate_only{
SOURCES += *.qml \
    menu/*.qml \
    tools/*.qml \
    tools/audio/*.qml \
    tools/maps/*.qml \
    tools/notes/*.qml \
    tools/settings/*.qml \
    tools/generators/*.qml
}

HEADERS  += gm-companion/functions.h \
    # UI
#    gm-companion/ui/whatisnewwindow.h \
#    gm-companion/ui/aboutdialog.h \
    # Other
    gm-companion/version.h \
    # Tools
    gm-companion/tools/maptool.h \
    gm-companion/tools/audio/audiotool.h \
    gm-companion/tools/audio/audioeditor.h \
    gm-companion/tools/dicetool.h \
    gm-companion/tools/combattracker.h \
    gm-companion/tools/notestool.h \
    gm-companion/tools/convertertool.h \
    # Tools CHARACTERS
#    gm-companion/tools/characters/charactertool.h \
#    gm-companion/tools/characters/sheets/defaultsheet.h \
#    gm-companion/tools/characters/sheets/sheetfunctions.h \
#    gm-companion/tools/characters/sheets/dsa5sheet.h \
    # Tools ADDONS
#    gm-companion/tools/addons/sifrp.h \
    # Tools GENERATORS
     gm-companion/tools/generators/namegenerator.h \
    # Tools ITEMS
#    gm-companion/tools/items/shoptool.h \
#    gm-companion/tools/items/shopeditor.h \
#    gm-companion/tools/items/itemeditor.h \
    # Managers
#    gm-companion/managers/filemanager.h \
    gm-companion/managers/updatemanager.h \
    # Settings
    gm-companion/settings/settingstool.h \
    gm-companion/settings/settingsmanager.h \
#    gm-companion/tools/addons/addonmanager.h \
#    gm-companion/tools/characters/sheets/entaria2sheet.h \
#    gm-companion/tools/characters/sheets/dnd5esheet.h
    gm-companion/platformdetails.h


DISTFILES +=

QML_IMPORT_PATH =

RC_FILE = gm-companion/GM-Companion.rc

RESOURCES += gm-companion/resources/resources.qrc \
    gm-companion/resources/styles/qdarkstyle/style.qrc \
    gm-companion/ui/qml.qrc

TRANSLATIONS+=  gm-companion/resources/translations/gm-companion_en.ts
TRANSLATIONS+=  gm-companion/resources/translations/gm-companion_de.ts

!win32 {
    QMAKE_CXXFLAGS += -std=c++11

    target.path = /usr/bin
    INSTALLS += target

    data.path = /usr/share/gm-companion
    data.files = data/share/gm-companion/*

    resources.path = /usr/share/gm-companion/resources
    resources.files += data/share/resources/*

    desktop.path = /usr/share/applications
    desktop.files = data/share/applications/*

    INSTALLS += data
    INSTALLS += desktop
    INSTALLS += resources
}

#unix:!macx: LIBS += -ltag
