QT += gui opengl

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(inc/inc.pri)
include(src/src.pri)
include(ui/ui.pri)

INCLUDEPATH += "/usr/local/include/eigen3/" \
               "/usr/local/include" \
               "lib/glm/" \
               "lib/libigl/include"

LIBS += -L/usr/local/Cellar/cgal/4.9.1/lib -lCGAL

RESOURCES += \
    resources.qrc

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -mmacosx-version-min=10.9
QMAKE_LFLAGS += -mmacosx-version-min=10.9

