INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
include(objects/objects.pri)
include(processor/processor.pri)
include(scenes/scenes.pri)
include(tools/tools.pri)
include(ui/ui.pri)
SOURCES +=  \
    $$PWD/main.cpp