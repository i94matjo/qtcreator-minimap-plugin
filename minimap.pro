TEMPLATE = lib

DEFINES += MINIMAP_LIBRARY

IDE_SOURCE_TREE = $$(QTC_SOURCE)
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_SOURCE_TREE):IDE_SOURCE_TREE=/home/ubuntu/software/qt-creator-opensource-src-4.1.0
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/ubuntu/qtcreator-4.1.0

QTC_PLUGIN_NAME = Minimap
QTC_PLUGIN_DEPENDS = coreplugin texteditor

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)

SOURCES += \
    minimap.cpp \
    minimapstyle.cpp \
    minimapsettings.cpp

HEADERS += \
    minimap_global.h \
    minimapconstants.h \
    minimap.h \
    minimapstyle.h \
    minimapsettings.h

FORMS +=

RESOURCES +=

OTHER_FILES +=

DISTFILES += \
    AUTHORS \
    COPYING \
    README.md
