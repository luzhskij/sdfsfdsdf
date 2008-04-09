TEMPLATE = app
QT += core gui opengl
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp
DEPENDPATH += . tmp
DESTDIR = bin

unix {
    CONFIG += debug_and_release build_all
}

HEADERS += glwidget3d.h \
 mgrid.h \
 mmodel.h \
 mobject.h \
 modeler.h \
 mpoint.h \
 mpoints.h \
 mrgb.h \
 newgrid.h \
 globals.h \
 treemodel.h \
 renderer.h \
 glrenderer3d.h \
 camera.h \
 mprogressbartracker.h \
 mabstractview.h \
 mview3d.h \
 treeitem.h \
 borderstruct.h \
 mfolder.h \
 mmodelproperty.h \
 mwell.h \
 mcolorgenerator.h
FORMS += modeler.ui newgrid.ui
SOURCES += glwidget3d.cpp \
 main.cpp \
 mgrid.cpp \
 mmodel.cpp \
 mobject.cpp \
 modeler.cpp \
 mpoints.cpp \
 newgrid.cpp \
 treemodel.cpp \
 renderer.cpp \
 glrenderer3d.cpp \
 camera.cpp \
 mprogressbartracker.cpp \
 mabstractview.cpp \
 mview3d.cpp \
 treeitem.cpp \
 mfolder.cpp \
 mmodelproperty.cpp \
 mwell.cpp \
 mcolorgenerator.cpp
RESOURCES += geoid.qrc
